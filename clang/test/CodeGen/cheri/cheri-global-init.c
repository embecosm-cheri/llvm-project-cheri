// RUN: %cheri_cc1 %s "-target-abi" "purecap" -emit-llvm  -o - | %cheri_FileCheck --check-prefix=CHECK %s

int a[5];
int *b[] = {&a[2], &a[1], a};
// CHECK: @b = addrspace(200) global [3 x ptr addrspace(200)] [ptr addrspace(200) getelementptr (i8, ptr addrspace(200) @a, i64 8), ptr addrspace(200) getelementptr (i8, ptr addrspace(200) @a, i64 4), ptr addrspace(200) @a], align [[#CAP_SIZE]]
// CHECK-NOT: __cxx_global_var_init
