// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi n64 %s -emit-llvm -o - | FileCheck %s

template <typename T> static inline T loadFromCap(T*  __capability address) {
  return *address;
}

// CHECK-LABEL: _Z3funU3capPi
// CHECK: call signext i32 @{{.*}}loadFromCap{{.*}}(i32 addrspace(200)*
int fun(int * __capability t) {
  return loadFromCap<int>(t);
}

// CHECK: loadFromCap
// CHECK: load i32, i32 addrspace(200)* %0
// CHECK: ret i32
