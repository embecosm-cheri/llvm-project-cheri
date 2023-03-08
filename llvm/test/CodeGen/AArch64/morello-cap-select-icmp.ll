; RUN: opt -march=arm64 -target-abi purecap -mattr=+morello,+c64 %s -o - -S -early-cse | FileCheck %s

target datalayout = "e-m:e-pf200:128:128:128:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64-none-unknown-elf"

; Capability compare is done by virtual address and ignores the capability
; metadata. Therefore optimizing this to %xx would be wrong, as it would ignore
; the possilbly different metadata from %yy.

; CHECK-LABEL: foo
; CHECK: %cmp = icmp eq i8 addrspace(200)* %xx, %yy
; CHECK: %yy.xx = select i1 %cmp, i8 addrspace(200)* %yy, i8 addrspace(200)* %xx
; CHECK: ret i8 addrspace(200)* %yy.xx
define dso_local i8 addrspace(200)* @foo(i8 addrspace(200)* %xx, i8 addrspace(200)* %yy) {
entry:
  %cmp = icmp eq i8 addrspace(200)* %xx, %yy
  %yy.xx = select i1 %cmp, i8 addrspace(200)* %yy, i8 addrspace(200)* %xx
  ret i8 addrspace(200)* %yy.xx
}
