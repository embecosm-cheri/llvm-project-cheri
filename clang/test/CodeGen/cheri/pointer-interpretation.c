// RUN: %cheri_cc1 %s -O2 -msoft-float -emit-llvm -o - | FileCheck %s
#pragma pointer_interpretation capability
struct test
{
	int *a,*b,*c;
};
#pragma pointer_interpretation default
struct test2
{
	int *a,*b,*c;
} t2;
#pragma pointer_interpretation integer
struct test3
{
	int *a,*b,*c;
} t3;
struct test t;

_Pragma("pointer_interpretation integer")
_Pragma("pointer_interpretation capability")
_Pragma("pointer_interpretation default")

//CHECK: %struct.test = type { ptr addrspace(200), ptr addrspace(200), ptr addrspace(200) }
//CHECK: %struct.test2 = type { ptr, ptr, ptr }
//CHECK: %struct.test3 = type { ptr, ptr, ptr }

// CHECK: define dso_local signext i32 @x()
int x(void)
{
	// CHECK: load ptr addrspace(200), ptr
	// CHECK: load i32, ptr addrspace(200)
	return *t.a;
}

// CHECK: define dso_local signext i32 @y()
int y(void)
{
	// CHECK: load ptr, ptr
	// CHECK: load i32, ptr
	return *t2.a;
}
// CHECK: define dso_local signext i32 @z()
int z(void)
{
	// CHECK: load ptr, ptr
	// CHECK: load i32, ptr
	return *t3.a;
}

