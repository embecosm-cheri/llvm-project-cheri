// RUN: %clang_cc1 -triple aarch64-none-elf -target-feature +c64 -target-abi purecap -emit-llvm -o - -mllvm -cheri-cap-table-abi=pcrel %s | FileCheck %s

// CHECK: @llvm.global_ctors = appending addrspace(200) global [2 x { i32, void () addrspace(200)*, i8 addrspace(200)* }] [{ i32, void () addrspace(200)*, i8 addrspace(200)* } { i32 65535, void () addrspace(200)* @A, i8 addrspace(200)* null }, { i32, void () addrspace(200)*, i8 addrspace(200)* } { i32 65535, void () addrspace(200)* @C, i8 addrspace(200)* null }]

// CHECK: @llvm.global_dtors = appending addrspace(200) global [2 x { i32, void () addrspace(200)*, i8 addrspace(200)* }] [{ i32, void () addrspace(200)*, i8 addrspace(200)* } { i32 65535, void () addrspace(200)* @B, i8 addrspace(200)* null }, { i32, void () addrspace(200)*, i8 addrspace(200)* } { i32 65535, void () addrspace(200)* @D, i8 addrspace(200)* null }]

int printf(const char *, ...);

void A() __attribute__((constructor));
void B() __attribute__((destructor));

void A() {
  printf("A\n");
}

void B() {
  printf("B\n");
}

static void C() __attribute__((constructor));

static void D() __attribute__((destructor));

static int foo() {
  return 10;
}

static void C() {
  printf("A: %d\n", foo());
}

static void D() {
  printf("B\n");
}

int main() {
  return 0;
}
