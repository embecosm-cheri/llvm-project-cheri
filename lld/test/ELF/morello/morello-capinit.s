// REQUIRES: aarch64
// RUN: llvm-mc --triple=aarch64-none-elf -mattr=+c64,+morello -filetype=obj %s -o %t.o
// RUN: ld.lld %t.o -o %t
// RUN: llvm-readobj --cap-relocs --symbols %t | FileCheck %s

/// Basics of the .capinit relocation using static linking.
/// We create two capabilites via .capinit. These will produce R_MORELLO_CAPINIT
/// Relocations that the linker will use to create the __cap_relocs section.
/// We also check that the linker creates the __cap_relocs_start and
/// __cap_relocs_end symbols that a C-library can use to initialise the
/// capabilities with.

 .data
 .balign 16
 .type ptr1, %object
 .size ptr1, 16
ptr1:
 .capinit str + 8
 .8byte 0
 .8byte 12

 .type ptr2, %object
 .size ptr2, 16
ptr2:
 .capinit str
 .8byte 0
 .8byte 12

str:
 .string "Hello World"

 .globl __cap_relocs_start
 .globl __cap_relocs_end
 .xword __cap_relocs_start
 .xword __cap_relocs_end

// CHECK:          Name: __cap_relocs_end
// CHECK-NEXT:     Value: 0x220218
// CHECK-NEXT:     Size: 0
// CHECK-NEXT:     Binding: Local (0x0)
// CHECK-NEXT:     Type: None (0x0)
// CHECK-NEXT:     Other [ (0x2)
// CHECK-NEXT:       STV_HIDDEN (0x2)
// CHECK-NEXT:     ]
// CHECK-NEXT:     Section: __cap_relocs
// CHECK-NEXT:   }
// CHECK-NEXT:   Symbol {
// CHECK-NEXT:     Name: __cap_relocs_start
// CHECK-NEXT:     Value: 0x2201C8
// CHECK-NEXT:     Size: 0
// CHECK-NEXT:     Binding: Local (0x0)
// CHECK-NEXT:     Type: None (0x0)
// CHECK-NEXT:     Other [ (0x2)
// CHECK-NEXT:       STV_HIDDEN (0x2)
// CHECK-NEXT:     ]
// CHECK-NEXT:     Section: __cap_relocs

// CHECK: CHERI __cap_relocs [
// CHECK-NEXT:    0x230220 (ptr1)          Base: 0x230240 (str+8) Length: 12 Perms: (RWDATA)
// CHECK-NEXT:    0x230230 (ptr2)          Base: 0x230240 (str+0) Length: 12 Perms: (RWDATA)
