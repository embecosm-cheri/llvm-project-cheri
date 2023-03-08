; RUN: opt < %s -inline -S | FileCheck %s
target datalayout = "e-m:e-pf200:128:128:128:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-A200-P200-G200"
target triple = "aarch64--linux-android"

$_Z4procIQPFvQPcEEvT_ = comdat any
$_Z4procIQPFvQPKcEEvT_ = comdat any

@s = external local_unnamed_addr addrspace(200) global i8 addrspace(200)*, align 16

define void @_Z5proc1v() local_unnamed_addr addrspace(200) {
; CHECK-LABEL: @_Z5proc1v
; CHECK: call void @_Z9ext_proc1QPc
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: call void @_Z3padv()
; CHECK-NEXT: ret void
entry:
  ;%0 = call i8 addrspace(200)* @llvm.cheri.cap.from.pcc(i64 ptrtoint (void (i8 addrspace(200)*)* @_Z9ext_proc1QPc to i64))
  ;%1 = bitcast i8 addrspace(200)* %0 to void (i8 addrspace(200)*) addrspace(200)*
  call void @_Z4procIQPFvQPcEEvT_(void (i8 addrspace(200)*) addrspace(200)* @_Z9ext_proc1QPc)
  ret void
}
; CHECK-NOT: @_Z4procIQPFvQPcEEvT_(void (i8 addrspace(200)*) addrspace(200)* %f) local_unnamed_addr comdat {

define linkonce_odr void @_Z4procIQPFvQPcEEvT_(void (i8 addrspace(200)*) addrspace(200)* %f) local_unnamed_addr addrspace(200) comdat {
entry:
  %0 = load i8 addrspace(200)*, i8 addrspace(200)* addrspace(200)* @s, align 16
  call void %f(i8 addrspace(200)* %0)
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  ret void
}

;declare i8 addrspace(200)* @llvm.cheri.cap.from.pcc(i64)

declare void @_Z9ext_proc1QPc(i8 addrspace(200)*) addrspace(200)

define void @_Z5proc2v() local_unnamed_addr addrspace(200) {
; CHECK-LABEL: @_Z5proc2v()
; CHECK: call void @_Z9ext_proc2QPKc
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: call void @_Z3padv
; CHECK-NEXT: ret void
entry:
 ; %0 = call i8 addrspace(200)* @llvm.cheri.cap.from.pcc(i64 ptrtoint (void (i8 addrspace(200)*)* @_Z9ext_proc2QPKc to i64))
 ; %1 = bitcast i8 addrspace(200)* %0 to void (i8 addrspace(200)*) addrspace(200)*
  call void @_Z4procIQPFvQPKcEEvT_(void (i8 addrspace(200)*) addrspace(200)* @_Z9ext_proc2QPKc)
  ret void
}

; CHECK-NOT: @_Z4procIQPFvQPKcEEvT_
define linkonce_odr void @_Z4procIQPFvQPKcEEvT_(void (i8 addrspace(200)*) addrspace(200)* %f) local_unnamed_addr addrspace(200) comdat {
entry:
  %0 = load i8 addrspace(200)*, i8 addrspace(200)* addrspace(200)* @s, align 16
  call void %f(i8 addrspace(200)* %0)
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  call void @_Z3padv()
  ret void
}

declare void @_Z9ext_proc2QPKc(i8 addrspace(200)*) addrspace(200)
declare void @_Z3padv() local_unnamed_addr addrspace(200) #2
