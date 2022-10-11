// RUN: %cheri_purecap_cc1 %s -O0 -msoft-float -emit-llvm -o - | FileCheck %s
// Ensure literals don't spontaneously switch address space during calls when
// using the pure capability ABI. A regression test for #5.

char takes_string_ptr(const char *a) {
    return a[0];
}

// CHECK-LABEL: main
int main(void) {
  // CHECK: call signext i8 @takes_string_ptr(ptr addrspace(200) noundef @.str)
  return takes_string_ptr("hi");
}
