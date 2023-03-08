; RUN: llc -march=arm64 -mattr=+c64,+morello -target-abi purecap -o - %s | FileCheck %s

target datalayout = "e-m:e-pf200:128:128:128:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-A200-P200-G200"
target triple = "aarch64-none-unknown-elf"

; CHECK-LABEL: foo
define dso_local i8 addrspace(200)* @foo(i8 addrspace(200)* nocapture readnone %bb, i8 addrspace(200)* %cc) local_unnamed_addr addrspace(200) {
entry:
; CHECK: mov c0, c1
  %call = tail call i8 addrspace(200)* @bar(i8 addrspace(200)* %cc)
  ret i8 addrspace(200)* %call
}

; CHECK-LABEL: baz
define dso_local i8 addrspace(200)* @baz() local_unnamed_addr addrspace(200) {
entry:
; CHECK: mov x0, #0
  ret i8 addrspace(200)* null
}

declare dso_local i8 addrspace(200)* @bar(i8 addrspace(200)*) local_unnamed_addr addrspace(200)
