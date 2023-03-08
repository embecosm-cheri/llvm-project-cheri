// RUN: %clang %s -O0 -target aarch64-none-elf -march=morello+c64 -mabi=purecap -o - -S -emit-llvm -fPIC | FileCheck %s

// Test structs
struct ff1 {
  void *t1;
  void *t2;
};

struct ff1 g1(struct ff1 f1);

// CHECK: define { i8 addrspace(200)*, i8 addrspace(200)* } @fun1({ i8 addrspace(200)*, i8 addrspace(200)* } %
// CHECK: ret { i8 addrspace(200)*, i8 addrspace(200)* }
struct ff1 fun1(struct ff1 f1) {
  void * t = f1.t1;
  f1.t1 = f1.t2;
  f1.t2 = t;
  return g1(f1);
}

struct ff2 {
  void *t1;
};

struct ff2 g2(struct ff2 f1);

// CHECK: define i8 addrspace(200)* @fun2(i8 addrspace(200)* %f1.coerce)
// CHECK: ret i8 addrspace(200)*
struct ff2 fun2(struct ff2 f1) {
  f1.t1 += 1;
  return g2(f1);
}

// CHECK: define { i8 addrspace(200)*, i8 addrspace(200)* } @fun3({ i8 addrspace(200)*, i8 addrspace(200)* } %f0.coerce, { i8 addrspace(200)*, i8 addrspace(200)* } %f2.coerce, { i8 addrspace(200)*, i8 addrspace(200)* } %f3.coerce, { i8 addrspace(200)*, i8 addrspace(200)* } %f1.coerce)
// CHECK: ret { i8 addrspace(200)*, i8 addrspace(200)* } %
struct ff1 fun3(struct ff1 f0, struct ff1 f2, struct ff1 f3, struct ff1 f1) {
  void * t = f1.t1;
  f1.t1 = f1.t2;
  f1.t2 = t;
  return g1(f1);
}

// CHECK: define { i8 addrspace(200)*, i8 addrspace(200)* } @fun4({ i8 addrspace(200)*, i8 addrspace(200)* } %f0.coerce, { i8 addrspace(200)*, i8 addrspace(200)* } %f2.coerce, { i8 addrspace(200)*, i8 addrspace(200)* } %
// CHECK: ret { i8 addrspace(200)*, i8 addrspace(200)* } %
struct ff1 fun4(struct ff1 f0, struct ff1 f2, struct ff1 f1) {
  void * t = f1.t1;
  f1.t1 = f1.t2;
  f1.t2 = t;
  return g1(f1);
}

struct ff3 {
  void *t1;
  int m1;
  int t2;
};

struct ff3 g3(struct ff3 f1);

// CHECK: define { i8 addrspace(200)*, i64 } @fun5({ i8 addrspace(200)*, i64 } %f0.coerce, { i8 addrspace(200)*, i64 } %f2.coerce, { i8 addrspace(200)*, i64 }
// CHECK: ret { i8 addrspace(200)*, i64 }
struct ff3 fun5(struct ff3 f0, struct ff3 f2, struct ff3 f1) {
  return g3(f1);
}

struct ff4 {
  int m1;
  void *t1;
  int t2;
};

struct ff4 g4(struct ff4 f1);

// CHECK: define void @fun6(%struct.ff4 addrspace(200)* noalias sret %agg.result, %struct.ff4 addrspace(200)* %f0, %struct.ff4 addrspace(200)* %f2, %struct.ff4 addrspace(200)* %f1)
// CHECK: ret void
struct ff4 fun6(struct ff4 f0, struct ff4 f2, struct ff4 f1) {
  return g4(f1);
}

// Test arrays + structs
struct ff5 {
  void *t1;
  int m1[2];
};

struct ff5 g5(struct ff5 f1);

// CHECK: define { i8 addrspace(200)*, i64 } @fun7({ i8 addrspace(200)*, i64 } %f0.coerce, { i8 addrspace(200)*, i64 } %f2.coerce, { i8 addrspace(200)*, i64 } %f1.coerce)
// CHECK: ret { i8 addrspace(200)*, i64 }
struct ff5 fun7(struct ff5 f0, struct ff5 f2, struct ff5 f1) {
  return g5(f1);
}

struct ff6 {
  void *t1;
  int m1[3];
};

struct ff6 g6(struct ff6 f1);

// CHECK: define void @fun8(%struct.ff6 addrspace(200)* noalias sret %agg.result, %struct.ff6 addrspace(200)* %f0, %struct.ff6 addrspace(200)* %f2, %struct.ff6 addrspace(200)* %f1)
// CHECK: ret void
struct ff6 fun8(struct ff6 f0, struct ff6 f2, struct ff6 f1) {
  return g6(f1);
}

// Test some unions
struct ff7 {
  union {
    struct ff5 t1;
    struct ff3 t2;
  } tt;
};

struct ff7 g7(struct ff7 f1);

// CHECK: define { i8 addrspace(200)*, i64 } @fun9({ i8 addrspace(200)*, i64 } %f0.coerce, { i8 addrspace(200)*, i64 } %f2.coerce, { i8 addrspace(200)*, i64 } %f1.coerce)
// CHECK: ret { i8 addrspace(200)*, i64 }
struct ff7 fun9(struct ff7 f0, struct ff7 f2, struct ff7 f1) {
  return g7(f1);
}

struct ff8 {
  union {
    void *__capability t1;
    int t2;
  } tt;
  int t3;
};

struct ff8 g8(struct ff8 f1);

// CHECK: define { i8 addrspace(200)*, i64 } @fun10({ i8 addrspace(200)*, i64 } %f0.coerce, { i8 addrspace(200)*, i64 } %f2.coerce, { i8 addrspace(200)*, i64 } %f1.coerce)
// CHECK: ret { i8 addrspace(200)*, i64 }
struct ff8 fun10(struct ff8 f0, struct ff8 f2, struct ff8 f1) {
  return g8(f1);
}

struct ff9 {
  union {
    void * t1;
    long double t2;
  } tt;
  int t3;
};

struct ff9 g9(struct ff9 f1);

// CHECK: define void @fun11(%struct.ff9 addrspace(200)* noalias sret %agg.result, %struct.ff9 addrspace(200)* %f0, %struct.ff9 addrspace(200)* %f2, %struct.ff9 addrspace(200)* %f1)
// CHECK: ret void
struct ff9 fun11(struct ff9 f0, struct ff9 f2, struct ff9 f1) {
  return g9(f1);
}
