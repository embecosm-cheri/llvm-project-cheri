// RUN: %clang -### -target aarch64-none-elf -march=morello -mabi=purecap -cheri-linker -O2 -S %s 2>&1 | FileCheck -check-prefix=CHECK-CHERI-LINKER %s
// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=purecap -O2 -S %s 2>&1 | FileCheck -check-prefix=CHECK-CHERI-LINKER %s

// CHECK-CHERI-LINKER: "-cheri-linker"

// RUN: %clang -### -target aarch64-none-elf -nodefaultlibs -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-NODEFAULTLIB %s
// CHECK-NODEFAULTLIB-NOT: "-lclang_rt.builtins-aarch64"
// CHECK-NODEFAULTLIB-NOT: "-lc"
// CHECK-NODEFAULTLIB-NOT: "-lrdimon"

// RUN: %clang -### -target aarch64-none-elf -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-RTLIB %s
// RUN: %clang -### -target aarch64-none-elf -nodefaultlibs -rtlib=compiler-rt -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-RTLIB %s
// CHECK-RTLIB: "-lclang_rt.builtins-aarch64"

// RUN: %clang -### -target aarch64-none-elf -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-NEWLIB %s
// RUN: %clang -### -target aarch64-none-elf -nodefaultlibs -lc -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-NEWLIB %s
// CHECK-NEWLIB: "--start-group" "-lc" "-lrdimon" "--end-group"

// RUN: %clang -### -target aarch64-none-elf -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-GCSECTIONS %s
// CHECK-GCSECTIONS: "--gc-sections"

// RUN: %clang -### -target aarch64-none-elf -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-STARTFILE %s
// RUN: %clang -### -target aarch64-none-elf -O1 -nostartfiles %s 2>&1 | FileCheck -check-prefix=CHECK-NOSTARTFILE %s
// RUN: %clang -### -target aarch64-none-elf -O1 -nostdlib %s 2>&1 | FileCheck -check-prefix=CHECK-NOSTARTFILE %s
// CHECK-STARTFILE: "{{.*}}/rdimon-crt0.o" "{{.*}}/crti.o" "{{.*}}cpu-init/rdimon-aem-el3.o"
// CHECK-STARTFILE: "{{.*}}/crtn.o"
// CHECK-NOSTARTFILE-NOT: "{{.*}}/rdimon-crt0.o"
// CHECK-NOSTARTFILE-NOT: "{{.*}}/crti.o"
// CHECK-NOSTARTFILE-NOT: "{{.*}}cpu-init/rdimon-aem-el3.o"
// CHECK-NOSTARTFILE-NOT: "{{.*}}/crtn.o"

// RUN: %clang -### -target aarch64-none-elf -O1 -fuse-ld=lld %s 2>&1 | FileCheck -check-prefix=CHECK-IMAGEBASE %s
// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=purecap -O1 %s 2>&1 | FileCheck -check-prefix=CHECK-SEARCHPATH %s
// CHECK-IMAGEBASE: "--image-base" "0x80000000"
// CHECK-SEARCHPATH: "-L" "{{.*}}/aarch64-none-elf+morello+c64+purecap/lib"
// CHECK-SEARCHPATH-NOT: "-T" "{{.*}}/ldscripts/aarch64elf-lld.x"

// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=purecap -capsizefix %s 2>&1 | FileCheck -check-prefix=CHECK-CAPSIZEFIX %s
// RUN: %clang -### -target aarch64-none-elf -march=morello -capsizefix %s 2>&1 | FileCheck -check-prefix=CHECK-CAPSIZEFIX %s
// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=purecap  %s 2>&1 | FileCheck -check-prefix=CHECK-NOCAPSIZEFIX %s
// RUN: %clang -### -target aarch64-none-elf -march=morello %s 2>&1 | FileCheck -check-prefix=CHECK-NOCAPSIZEFIX %s

// CHECK-CAPSIZEFIX: "-emit-no-morello-relocations"
// CHECK-CAPSIZEFIX: "{{.*}}capsizefix" "{{.*}}"
// CHECK-NOCAPSIZEFIX-NOT: "{{.*}}capsizefix" "{{.*}}"

// RUN: %clang -### -target aarch64-none-elf %s 2>&1 | FileCheck -check-prefix=CHECK-DEFAULT-LINKER %s
// CHECK-DEFAULT-LINKER: "{{.*}}ld.lld"
// RUN: %clang -### -target aarch64-none-elf %s -fuse-ld=bfd 2>&1 | FileCheck -check-prefix=CHECK-USE-BFDLINKER %s
// CHECK-USE-BFDLINKER: "{{.*}}aarch64-none-elf-ld.bfd"
// RUN: %clang -### -target aarch64-none-elf %s -fuse-ld=lld 2>&1 | FileCheck -check-prefix=CHECK-USE-LLDLINKER %s
// CHECK-USE-LLDLINKER: "{{.*}}ld.lld"
//
// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=purecap %s 2>&1 | FileCheck -check-prefix=C64-PLT %s
// RUN: %clang -### -target aarch64-none-elf -march=morello -mabi=purecap %s 2>&1 | FileCheck -check-prefix=C64-PLT %s
// RUN: %clang -### -target aarch64-unknown-freebsd12.0  -mabi=purecap %s 2>&1 | FileCheck -check-prefix=C64-PLT %s
// C64-PLT: --morello-c64-plt

//
// RUN: %clang -### -target aarch64-none-elf -march=morello %s 2>&1 | FileCheck -check-prefix=NOC64-PLT %s
// RUN: %clang -### -target aarch64-linux-elf -march=armv8-a+a64c %s 2>&1 | FileCheck -check-prefix=NOC64-PLT %s
// RUN: %clang -### -target aarch64-none-elf -march=morello+c64 -mabi=aapcs %s 2>&1 | FileCheck -check-prefix=NOC64-PLT %s

// NOC64-PLT-NOT: --morello-c64-plt

// RUN: %clang -### -target aarch64-unknown-freebsd12.0  -march=morello+c64 -mabi=purecap %s --sysroot=%S/Inputs/basic_cheribsd_libcheri_tree 2>&1| FileCheck --check-prefix=LIBCHERI %s
// LIBCHERI: --sysroot=[[SYSROOT:[^"]+]]
// LIBCHERI: "-L[[SYSROOT]]/usr/libcheri"
