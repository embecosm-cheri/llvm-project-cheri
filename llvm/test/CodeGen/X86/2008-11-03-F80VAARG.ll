; RUN: llc < %s -mtriple=i686-- -o - | FileCheck %s

declare void @llvm.va_start.p0(ptr) nounwind

declare void @llvm.va_copy.p0.p0(ptr, ptr) nounwind

declare void @llvm.va_end.p0(ptr) nounwind

; CHECK-LABEL: test:
; CHECK-NOT: 10
define x86_fp80 @test(...) nounwind {
	%ap = alloca ptr		; <ptr> [#uses=3]
	call void @llvm.va_start.p0(ptr %ap)
	%t1 = va_arg ptr %ap, x86_fp80		; <x86_fp80> [#uses=1]
	%t2 = va_arg ptr %ap, x86_fp80		; <x86_fp80> [#uses=1]
	%t = fadd x86_fp80 %t1, %t2		; <x86_fp80> [#uses=1]
	ret x86_fp80 %t
}
