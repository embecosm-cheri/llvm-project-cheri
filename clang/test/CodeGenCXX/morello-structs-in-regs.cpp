// RUN: %clang %s -target aarch64-none-elf -march=morello+c64 -mabi=purecap -o - -emit-llvm -S -fPIC | FileCheck %s

// We can see the base classes.
class Foo {
public:
  void *tt;
  int qq[3];
};

class FooD : public Foo  {};

void FooDCallee(FooD, FooD);

// CHECK: define void @_Z10FooDCaller4FooD(%class.FooD addrspace(200)* %t1)
void FooDCaller(FooD t1) {
  FooDCallee(t1, t1);
}

class Bar {
public:
  void *tt;
  int qq[2];
};

class BarD : public Bar {};

void BarDCallee(BarD, BarD);

// CHECK: define void @_Z10BarDCaller4BarD({ i8 addrspace(200)*, i64 } %t1.coerce)
void BarDCaller(BarD t1) {
  BarDCallee(t1, t1);
}

// We need to check for the vtable pointer.
class Baz {
public:
  void *tt;
  int qq[2];
  virtual void virtFun();
};

class BazD : public Baz {};

void BazDCallee(BazD, BazD);

// CHECK: define void @_Z10BazDCaller4BazD(%class.BazD addrspace(200)* %t1)
void BazDCaller(BazD t1) {
  BazDCallee(t1, t1);
}

class Baf {
public:
  void *tt;
  virtual void virtFun();
};

class BafD : public Baf {};

void BafDCallee(BafD, BafD);

// CHECK: define void @_Z10BafDCaller4BafD(%class.BafD addrspace(200)* %t1)
void BafDCaller(BafD t1) {
  BafDCallee(t1, t1);
}
