// RUN: llvm-mc -triple=arm64 -mattr=+morello -show-encoding < %s | FileCheck %s
// RUN: llvm-mc -triple=arm64 -mattr=+morello,+c64 -show-encoding < %s | FileCheck %s

  csel c6, c25, c5, eq
  csel c6, c25, c5, ne
  csel c6, c25, c5, cs
  csel c6, c25, c5, hs
  csel c25, c6, c26, cc
  csel c25, c6, c26, lo
  csel c6, c25, c5, mi
  csel c6, c25, c5, pl
  csel c6, c25, c5, vs
  csel c6, c25, c5, vc
  csel c6, c25, c5, hi
  csel c6, c25, c5, ls
  csel c6, c25, c5, ge
  csel c6, c25, c5, lt
  csel c6, c25, c5, gt
  csel c6, c25, c5, le
  csel c6, c25, c5, al
  csel c6, c25, c5, nv

// CHECK: csel c6, c25, c5, eq  // encoding: [0x26,0x0f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, ne  // encoding: [0x26,0x1f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, hs  // encoding: [0x26,0x2f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, hs  // encoding: [0x26,0x2f,0xc5,0xc2]
// CHECK: csel c25, c6, c26, lo // encoding: [0xd9,0x3c,0xda,0xc2]
// CHECK: csel c25, c6, c26, lo // encoding: [0xd9,0x3c,0xda,0xc2]
// CHECK: csel c6, c25, c5, mi  // encoding: [0x26,0x4f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, pl  // encoding: [0x26,0x5f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, vs  // encoding: [0x26,0x6f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, vc  // encoding: [0x26,0x7f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, hi  // encoding: [0x26,0x8f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, ls  // encoding: [0x26,0x9f,0xc5,0xc2]
// CHECK: csel c6, c25, c5, ge  // encoding: [0x26,0xaf,0xc5,0xc2]
// CHECK: csel c6, c25, c5, lt  // encoding: [0x26,0xbf,0xc5,0xc2]
// CHECK: csel c6, c25, c5, gt  // encoding: [0x26,0xcf,0xc5,0xc2]
// CHECK: csel c6, c25, c5, le  // encoding: [0x26,0xdf,0xc5,0xc2]
// CHECK: csel c6, c25, c5, al  // encoding: [0x26,0xef,0xc5,0xc2]
// CHECK: csel c6, c25, c5, nv  // encoding: [0x26,0xff,0xc5,0xc2]

  add c28, c4, #1329
  add c28, c4, #2766, lsl #12
  add c28, c27, #1329, lsl #0
  add c28, c27, #2766, lsl #12
  add c3, c4, #1329
  add c3, c4, #2766, lsl #12
  add c3, c27, #1329
  add c3, c27, #2766, lsl #12
  add c3, c27, #4095
  add c3, c27, #4095, lsl #12
  add c3, c27, #4096
// CHECK: add c28, c4, #1329            // encoding: [0x9c,0xc4,0x14,0x02]
// CHECK: add c28, c4, #2766, lsl #12   // encoding: [0x9c,0x38,0x6b,0x02]
// CHECK: add c28, c27, #1329           // encoding: [0x7c,0xc7,0x14,0x02]
// CHECK: add c28, c27, #2766, lsl #12  // encoding: [0x7c,0x3b,0x6b,0x02]
// CHECK: add c3, c4, #1329             // encoding: [0x83,0xc4,0x14,0x02]
// CHECK: add c3, c4, #2766, lsl #12    // encoding: [0x83,0x38,0x6b,0x02]
// CHECK: add c3, c27, #1329            // encoding: [0x63,0xc7,0x14,0x02]
// CHECK: add c3, c27, #2766, lsl #12   // encoding: [0x63,0x3b,0x6b,0x02]
// CHECK: add c3, c27, #4095            // encoding: [0x63,0xff,0x3f,0x02]
// CHECK: add c3, c27, #4095, lsl #12   // encoding: [0x63,0xff,0x7f,0x02]
// CHECK: add c3, c27, #1, lsl #12      // encoding: [0x63,0x07,0x40,0x02]

  sub c25, csp, #689
  sub c25, csp, #3406, lsl #12
  sub c25, c0, #689, lsl #0
  sub c25, c0, #3406, lsl #12
  sub c6, csp, #689
  sub c6, csp, #3406, lsl #12
  sub c6, c0, #689
  sub c6, c0, #3406, lsl #12
  sub c6, c0, #4095
  sub c6, c0, #4095, lsl #12
  sub c6, c0, #4096
// CHECK: sub c25, csp, #689            // encoding: [0xf9,0xc7,0x8a,0x02]
// CHECK: sub c25, csp, #3406, lsl #12  // encoding: [0xf9,0x3b,0xf5,0x02]
// CHECK: sub c25, c0, #689             // encoding: [0x19,0xc4,0x8a,0x02]
// CHECK: sub c25, c0, #3406, lsl #12   // encoding: [0x19,0x38,0xf5,0x02]
// CHECK: sub c6, csp, #689             // encoding: [0xe6,0xc7,0x8a,0x02]
// CHECK: sub c6, csp, #3406, lsl #12   // encoding: [0xe6,0x3b,0xf5,0x02]
// CHECK: sub c6, c0, #689              // encoding: [0x06,0xc4,0x8a,0x02]
// CHECK: sub c6, c0, #3406, lsl #12    // encoding: [0x06,0x38,0xf5,0x02]
// CHECK: sub c6, c0, #4095             // encoding: [0x06,0xfc,0xbf,0x02]
// CHECK: sub c6, c0, #4095, lsl #12    // encoding: [0x06,0xfc,0xff,0x02]
// CHECK: sub c6, c0, #1, lsl #12       // encoding: [0x06,0x04,0xc0,0x02]

  add c6, c25, w7, uxtb #0
  add csp, csp, w8, uxth #1
  add c25, c6, w16, uxtw #2
  add csp, c6, x24, uxtx #3
  add c5, c7, w3, sxtb #4
  add c4, c24, wzr, sxth #0
  add c3, c27, w23,  sxtw #1
  add c7, c28, x6, sxtx #2
  add c0, c0, x0
// CHECK: add c6, c25, w7, uxtb         // encoding: [0x26,0x03,0xa7,0xc2]
// CHECK: add csp, csp, w8, uxth #1     // encoding: [0xff,0x27,0xa8,0xc2]
// CHECK: add c25, c6, w16, uxtw #2     // encoding: [0xd9,0x48,0xb0,0xc2]
// CHECK: add csp, c6, x24, uxtx #3     // encoding: [0xdf,0x6c,0xb8,0xc2]
// CHECK: add c5, c7, w3, sxtb #4       // encoding: [0xe5,0x90,0xa3,0xc2]
// CHECK: add c4, c24, wzr, sxth        // encoding: [0x04,0xa3,0xbf,0xc2]
// CHECK: add c3, c27, w23,  sxtw #1    // encoding: [0x63,0xc7,0xb7,0xc2]
// CHECK: add c7, c28, x6, sxtx #2      // encoding: [0x87,0xeb,0xa6,0xc2]
// CHECK: add c0, c0, x0, uxtx          // encoding: [0x00,0x60,0xa0,0xc2]

  scbnds c7, c3, #0
  scbnds c28, c24, #80
  scbnds c28, c24, #5, lsl #4
  scbnds c0, c7, #1008
  scbnds c0, c7, #63, lsl #4
  scbnds csp, csp, #49
  scbnds csp, csp, #49, lsl #0
  scbnds csp, csp, #784
  scbnds c4, c25, #14
  scbnds c4, c25, #16
// CHECK: scbnds c7, c3, #0             // encoding: [0x67,0x38,0xc0,0xc2]
// CHECK: scbnds c28, c24, #5, lsl #4   // encoding: [0x1c,0xfb,0xc2,0xc2]
// CHECK: scbnds c28, c24, #5, lsl #4   // encoding: [0x1c,0xfb,0xc2,0xc2]
// CHECK: scbnds c0, c7, #63, lsl #4    // encoding: [0xe0,0xf8,0xdf,0xc2]
// CHECK: scbnds c0, c7, #63, lsl #4    // encoding: [0xe0,0xf8,0xdf,0xc2]
// CHECK: scbnds csp, csp, #49          // encoding: [0xff,0xbb,0xd8,0xc2]
// CHECK: scbnds csp, csp, #49          // encoding: [0xff,0xbb,0xd8,0xc2]
// CHECK: scbnds csp, csp, #49, lsl #4  // encoding: [0xff,0xfb,0xd8,0xc2]
// CHECK: scbnds c4, c25, #14           // encoding: [0x24,0x3b,0xc7,0xc2]
// CHECK: scbnds c4, c25, #16           // encoding: [0x24,0x3b,0xc8,0xc2]
