// RUN: %cheri_purecap_cc1 -emit-llvm -o - %s | FileCheck %s

struct _FILE;
typedef struct _FILE FILE;
int	 fprintf(FILE* __restrict, const char * __restrict, ...);

void __assert() {
  // CHECK: call signext i32 (ptr addrspace(200), ptr addrspace(200), ...) @fprintf(ptr addrspace(200) noundef null, ptr addrspace(200) noundef @.str)
  fprintf(0, "Assertion failed:");
}
