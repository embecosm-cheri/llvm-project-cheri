//===- MorelloRangeChecker.cpp ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringSwitch.h"
#include "llvm/IR/AbstractCallSite.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

#include <string>
#include <utility>

#include "llvm/IR/Verifier.h"

using namespace llvm;
using std::pair;

// FIXME: this is duplicated for the CheriRangeChecker in the Mips
// backend. This should be a generic pass, but we can't do this
// without making this more difficult to merge. Once the CheriRangeChecker
// is generic, remove this pass.

namespace {
class MorelloRangeChecker : public FunctionPass,
                            public InstVisitor<MorelloRangeChecker> {
  // Operands for an allocation.  Either one or two integers (constant or
  // variable).  If there are two, then they must be multiplied together.
  typedef pair<Value *, Value *> AllocOperands;
  struct ConstantCast {
    Instruction *Instr;
    unsigned OpNo;
    User *Origin;
  };
  DataLayout *TD = nullptr;
  Module *M = nullptr;
  IntegerType *Int64Ty = nullptr;
  PointerType *CapPtrTy = nullptr;
  SmallVector<pair<AllocOperands, Instruction *>, 32> Casts;
  SmallVector<pair<AllocOperands, ConstantCast>, 32> ConstantCasts;
  Function *SetLengthFn = nullptr;

  AllocOperands getRangeForAllocation(Use *Src) {
    AbstractCallSite Malloc = AbstractCallSite(Src);
    if (Malloc) {
      Function *Fn = Malloc.getCalledFunction();
      if (!Fn)
        return AllocOperands();
      switch (StringSwitch<int>(Fn->getName())
                  .Case("malloc", 1)
                  .Case("valloc", 1)
                  .Case("realloc", 2)
                  .Case("aligned_alloc", 2)
                  .Case("reallocf", 2)
                  .Case("calloc", 3)
                  .Default(-1)) {
      default:
        return AllocOperands();
      case 1:
        return AllocOperands(Malloc.getCallArgOperand(0), 0);
      case 2:
        return AllocOperands(Malloc.getCallArgOperand(1), 0);
      case 3:
        return AllocOperands(Malloc.getCallArgOperand(0), Malloc.getCallArgOperand(1));
      }
    } else if (AllocaInst *AI = dyn_cast<AllocaInst>(Src)) {
      PointerType *AllocaTy = AI->getType();
      Value *ArraySize = AI->getArraySize();
      Type *AllocationTy = AllocaTy->getElementType();
      unsigned ElementSize = TD->getTypeAllocSize(AllocationTy);
      if (ElementSize == 1)
        return AllocOperands(ArraySize, 0);
      Value *Size = ConstantInt::get(ArraySize->getType(), ElementSize);
      return AllocOperands(Size, ArraySize);
    }
    return AllocOperands();
  }
  Value *RangeCheckedValue(Instruction *InsertPt, AllocOperands AO, Value *I2P,
                           Value *&BitCast) {
    IRBuilder<> B(InsertPt);
    Value *Size = (AO.second) ? B.CreateMul(AO.first, AO.second) : AO.first;
    BitCast = B.CreateBitCast(I2P, CapPtrTy);
    if (Size->getType() != Int64Ty)
      Size = B.CreateZExt(Size, Int64Ty);
    CallInst *SetLength = B.CreateCall(SetLengthFn, {BitCast, Size});
    if (BitCast == I2P)
      BitCast = SetLength;
    return B.CreateBitCast(SetLength, I2P->getType());
  }

public:
  static char ID;
  MorelloRangeChecker() : FunctionPass(ID) {}
  virtual llvm::StringRef getPassName() const override {
    return "Morello range checker";
  }
  virtual bool doInitialization(Module &Mod) override {
    M = &Mod;
    TD = new DataLayout(M);
    Int64Ty = IntegerType::get(M->getContext(), 64);
    CapPtrTy = PointerType::get(IntegerType::get(M->getContext(), 8), 200);
    return true;
  }
  virtual ~MorelloRangeChecker() { delete TD; }
  bool checkOpcode(Value *V, unsigned Opcode) {
    if (Instruction *I = dyn_cast<Instruction>(V))
      return I->getOpcode() == Opcode;
    if (ConstantExpr *E = dyn_cast<ConstantExpr>(V))
      return E->getOpcode() == Opcode;
    return false;
  }
  User *testI2P(User &I2P) {
    PointerType *DestTy = dyn_cast<PointerType>(I2P.getType());
    if (DestTy && DestTy->getAddressSpace() == 200) {
      if (checkOpcode(I2P.getOperand(0), Instruction::PtrToInt)) {
        User *P2I = cast<User>(I2P.getOperand(0));
        PointerType *SrcTy =
            dyn_cast<PointerType>(P2I->getOperand(0)->getType());
        if (SrcTy && SrcTy->getAddressSpace() == 0) {
          Value *Src = P2I->getOperand(0)->stripPointerCasts();
          if (GlobalVariable *GV = dyn_cast<GlobalVariable>(Src))
            return GV->hasExternalLinkage() ? 0 : P2I;
          if (isa<AllocaInst>(Src) || AbstractCallSite(&P2I->getOperandUse(0))) {
            return P2I;
          }
        }
      }
    }
    return 0;
  }
  void visitAddrSpaceCast(AddrSpaceCastInst &ASC) {
    PointerType *DestTy = dyn_cast<PointerType>(ASC.getType());
    Value *Src = ASC.getOperand(0);
    PointerType *SrcTy = dyn_cast<PointerType>(Src->getType());
    if ((DestTy && DestTy->getAddressSpace() == 200) &&
        (SrcTy && SrcTy->getAddressSpace() == 0)) {
      Src = Src->stripPointerCasts();
      if (GlobalVariable *GV = dyn_cast<GlobalVariable>(Src)) {
        if (GV->hasExternalLinkage())
          return;
      } else if (!(isa<AllocaInst>(Src) || AbstractCallSite(&ASC.getOperandUse(0))))
        return;
      AllocOperands AO = getRangeForAllocation(&ASC.getOperandUse(0));
      if (AO != AllocOperands())
        Casts.push_back(pair<AllocOperands, Instruction *>(AO, &ASC));
    }
  }
  void visitIntToPtrInst(IntToPtrInst &I2P) {
    if (User *P2I = testI2P(I2P)) {
      Value *Src = P2I->getOperand(0)->stripPointerCasts();
      AllocOperands AO = getRangeForAllocation(&P2I->getOperandUse(0));
      if (AO != AllocOperands())
        Casts.push_back(pair<AllocOperands, Instruction *>(AO, &I2P));
    }
  }
  void visitRet(ReturnInst &RI) {
    Value *RV = RI.getReturnValue();
    if (RV && isa<ConstantExpr>(RV)) {
      ConstantCast C = {&RI, 0, testI2P(*cast<User>(RV))};
      if (C.Origin) {
        Value *Src = C.Origin->getOperand(0)->stripPointerCasts();
        AllocOperands AO = getRangeForAllocation(&C.Origin->getOperandUse(0));
        if (AO != AllocOperands())
          ConstantCasts.push_back(pair<AllocOperands, ConstantCast>(AO, C));
      }
    }
  }
  void visitCall(CallInst &CI) {
    for (unsigned i = 0; i < CI.getNumOperands(); i++) {
      Value *AV = CI.getOperand(i);
      if (AV && isa<ConstantExpr>(AV)) {
        ConstantCast C = {&CI, i, testI2P(*cast<User>(AV))};
        if (C.Origin) {
          Value *Src = C.Origin->getOperand(0)->stripPointerCasts();
          AllocOperands AO = getRangeForAllocation(&C.Origin->getOperandUse(0));
          if (AO != AllocOperands())
            ConstantCasts.push_back(pair<AllocOperands, ConstantCast>(AO, C));
        }
      }
    }
  }
  virtual bool runOnFunction(Function &F) override {
    Casts.clear();
    ConstantCasts.clear();

    visit(F);

    if (!(Casts.empty() && ConstantCasts.empty())) {
      Intrinsic::ID SetLength = Intrinsic::cheri_cap_bounds_set_exact;
      Type *SizeTy = Type::getIntNTy(
          M->getContext(), M->getDataLayout().getIndexSizeInBits(200));
      SetLengthFn = Intrinsic::getDeclaration(M, SetLength, SizeTy);
      Value *BitCast = 0;

      for (auto *i = Casts.begin(), *e = Casts.end(); i != e; ++i) {
        Instruction *I2P = i->second;
        auto InsertPt = I2P->getParent()->begin();
        while (&(*InsertPt) != I2P) {
          ++InsertPt;
        }
        ++InsertPt;
        Value *New = RangeCheckedValue(&*InsertPt, i->first, I2P, BitCast);
        I2P->replaceAllUsesWith(New);
        cast<Instruction>(BitCast)->setOperand(0, I2P);
      }
      for (pair<AllocOperands, ConstantCast> *i = ConstantCasts.begin(),
                                             *e = ConstantCasts.end();
           i != e; ++i) {
        Value *I2P = i->second.Instr->getOperand(i->second.OpNo);
        Value *New = RangeCheckedValue(i->second.Instr, i->first, I2P, BitCast);
        i->second.Instr->setOperand(i->second.OpNo, New);
      }
      return true;
    }
    return false;
  }
};
} // namespace

char MorelloRangeChecker::ID;

namespace llvm {
FunctionPass *createMorelloRangeChecker(void) {
  return new MorelloRangeChecker();
}
} // namespace llvm
