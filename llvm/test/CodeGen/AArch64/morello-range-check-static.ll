; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+morello -aarch64-enable-atomic-cfg-tidy=0 -disable-lsr -verify-machineinstrs -o - %s -relocation-model=pic  | FileCheck %s

target datalayout = "e-m:e-pf200:128:128:128:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"

%struct.example = type { i32 }

@example_object = internal addrspace(200) global %struct.example zeroinitializer, align 4

; CHECK-LABEL: static_bounds
define %struct.example addrspace(200)* @static_bounds() {
entry:
; CHECK:      adrp x[[ADDR:[0-9]+]], .LCPI0_0
; CHECK-NEXT: ldr c0, [x[[ADDR]], :lo12:.LCPI0_0]
  ret %struct.example addrspace(200)* @example_object
}

; CHECK-LABEL: alloca_cast_bounds
define void @alloca_cast_bounds() {
entry:
; CHECK: mov w[[SIZE:[0-9]+]], #1200
; CHECK: scbndse c0, c0, x[[SIZE]]

  %a = alloca [300 x i32], align 4
  %0 = bitcast [300 x i32] * %a to i8*
  %1 = addrspacecast i8 *%0 to i8 addrspace(200)*
  call void @bar(i8 addrspace(200)* %1)
  ret void
}

declare void @bar(i8 addrspace(200)*)
