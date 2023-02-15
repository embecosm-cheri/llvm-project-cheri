// Check that the printf -> puts optimization doesn't introduce AS0 strings
// RUN: %cheri_purecap_cc1 -o - -O2 -emit-llvm %s | FileCheck %s
// CHECK: @str = private unnamed_addr addrspace(200) constant [14 x i8] c"Hello, world!\00"

extern int printf(const char*, ...);

void foo(void) {
  printf("Hello, world!\n");
  // CHECK: tail call i32 @puts(ptr addrspace(200) nonnull @str)
}

