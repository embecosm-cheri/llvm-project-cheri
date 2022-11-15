// RUN: %cheri_cc1 -emit-llvm -o - %s | FileCheck %s

int main(void) {
  __intcap_t *t;
  // CHECK: {{%.+}} = load ptr, ptr %t
  if (t) {
  }
  return 0;
}
