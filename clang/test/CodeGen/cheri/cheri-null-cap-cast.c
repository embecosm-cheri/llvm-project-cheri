// RUN: %cheri_cc1 -emit-llvm -o - %s | FileCheck %s

int main(void) {
  char * __capability p;
  // CHECK: [[P:%.+]] = alloca ptr addrspace(200),
  // CHECK: [[VAR0:%.+]] = load ptr addrspace(200), ptr [[P]]
  // CHECK-NEXT: [[CMP:%.+]] = icmp eq ptr addrspace(200) [[VAR0]], null
  if (p == (void * __capability)0) {
  }
  // CHECK: [[VAR1:%.+]] = load ptr addrspace(200), ptr [[P]]
  // CHECK-NEXT: [[CMP1:%.+]] = icmp eq ptr addrspace(200) null, [[VAR1]]
  else if ((void * __capability)0 == p) {
  }
  return 0;
}
