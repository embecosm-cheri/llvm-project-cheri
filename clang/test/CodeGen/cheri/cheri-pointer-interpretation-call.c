// RUN: %cheri_cc1 -o - %s -emit-llvm | FileCheck %s
#define CHERI_CCALL(suffix, cls) \
	__attribute__((cheri_ccall))\
	__attribute__((cheri_method_suffix(suffix)))\
	__attribute__((cheri_method_class(cls)))

struct cheri_class
{
  void * __capability a;
  void * __capability b;
};

struct cheri_class def;
struct cheri_class other;


CHERI_CCALL("_cap", def)
__attribute__((pointer_interpretation_capabilities))
void *foo(void*, void*);

CHERI_CCALL("_cap", def)
void *baz(void*, void*);

void *bar(void *a, void *b)
{
	// CHECK: call chericcallcc ptr @cheri_invoke(ptr addrspace(200) noundef %{{.*}}, ptr addrspace(200) noundef %{{.*}}, i64 noundef zeroext %{{.*}}, ptr noundef %{{.*}}, ptr noundef %{{.*}})
	baz(a, b);
	// CHECK: [[CALL:%.+]] = call chericcallcc ptr addrspace(200) @cheri_invoke(ptr addrspace(200) noundef %{{.*}}, ptr addrspace(200) noundef %{{.*}}, i64 noundef zeroext %{{.*}}, ptr addrspace(200) noundef %{{.*}}, ptr addrspace(200) noundef %{{.*}})
	// CHECK:  %{{.*}} = addrspacecast ptr addrspace(200) [[CALL]] to ptr
	return (__cheri_fromcap void*)foo((__cheri_tocap void * __capability)a, (__cheri_tocap void * __capability)b);
}

