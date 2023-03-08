// REQUIRES: aarch64
// RUN: llvm-mc --triple=aarch64-none-elf %s -mattr=+c64,+morello --filetype=obj -o %t.o
// RUN: echo "SECTIONS { \
// RUN:       .text 0xff0000 : { *(.text.1) } \
// RUN:       .text_space : { *(.text.2) } \
// RUN:       .text_targets : { *(.text.3) *(.text.4) } \
// RUN:       } " > %t.script
// RUN: ld.lld %t.o -o %t --script=%t.script --morello-c64-plt -zmax-page-size=4096 2>&1 | FileCheck --check-prefix=WARN %s
// RUN: llvm-objdump --no-show-raw-insn -d --triple=aarch64-none-elf -mattr=+morello --start-address=0xff0000 --stop-address=0xff0008 %t | FileCheck %s
// RUN: ld.lld %t.o -o %t2 --script=%t.script -zmax-page-size=4096
// RUN: llvm-objdump --no-show-raw-insn -d --triple=aarch64-none-elf -mattr=+morello --start-address=0xff0000 --stop-address=0xff0008 %t2 | FileCheck %s

/// Check that the alignment of the PCC is not done when explicit addresses are specified
/// by the linker script.
 .section .text.1, "ax", %progbits
 .balign 4
 .global _start
 .type _start, %function
 .size _start, 4
_start:
 bl target
 b target2

 .section .text.2, "ax", %progbits
 .balign 1048576
 .space 127 * 1024 * 1024

 .section .text.3, "ax", %progbits
 .global target
 .type target, %function
 .size target, 4
target:
 ret

 .section .text.4, "ax", %progbits
 .global target2
 .type target2, %function
 .size target2, 4
target2:
 ret

/// Although additional alignment is not done if the linker script provides
/// explicit, warn about the potential misalignment.
// WARN: address (0xff0000) of section .text is not a multiple of alignment (262144)

/// Without additional alignment for PCC there are no Thunks needed.
// CHECK: 0000000000ff0000 _start:
// CHECK-NEXT:   ff0000:        bl      #133234688
// CHECK-NEXT:   ff0004:        b       #133234688

/// Check that specifying alignment of the PCC in the linker script overrides
/// above behaviour and influences other address sensitive content generation
/// such as range extension thunks.

// RUN: echo "SECTIONS { \
// RUN:       . = 0xff0000; \
// RUN:       .text : { *(.text.1) } \
// RUN:       .text_space : { *(.text.2) } \
// RUN:       .text_targets : { *(.text.3) *(.text.4) } \
// RUN:       } " > %t.script3
// RUN: ld.lld %t.o -o %t3 --script=%t.script3 --morello-c64-plt -zmax-page-size=4096
// RUN: llvm-objdump --no-show-raw-insn -d --triple=aarch64-none-elf -mattr=+morello --start-address=0x1000000 --stop-address=0x1000020 %t3 | FileCheck %s --check-prefix=ALIGN

/// When we align the PCC we incur additional alignment for .text.2 which
/// forces the creation of range extension thunks.
// ALIGN: 0000000001000000 __C64ADRPThunk_target:
// ALIGN-NEXT:  1000000:        adrp    c16, #134217728
// ALIGN-NEXT:  1000004:        add     c16, c16, #1
// ALIGN-NEXT:  1000008:        br      c16

// ALIGN: 000000000100000c __C64ADRPThunk_target2:
// ALIGN-NEXT:  100000c:        adrp    c16, #134217728
// ALIGN-NEXT:  1000010:        add     c16, c16, #5
// ALIGN-NEXT:  1000014:        br      c16

// ALIGN: 0000000001000018 _start:
// ALIGN-NEXT:  1000018:        bl      #-24 <__C64ADRPThunk_target>
// ALIGN-NEXT:  100001c:        b       #-16 <__C64ADRPThunk_target2>
