// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vmem_write_chunk.c
 */

#include "../include/vdp.h"

#if (__SDCCCALL == 1)

void vmem_write_chunk_1(const uint8_t * p, uint16_t n) {
  (void)p;                      // -> HL
  (void)n;                      // -> DE
  __asm__("        ld  c, #0x98");
  __asm__("        ld  a, d");
  __asm__("        or  a");
  __asm__("        jr  z, 00002$");
  __asm__("        ld  b, #0");
  __asm__("00001$: outi");           // 16 states
  __asm__("        jr  nz, 00001$"); // 12 or 7 states
  __asm__("        dec d");
  __asm__("        jr  nz, 00001$");
  __asm__("00002$: ld  b, e");
  __asm__("        ld  a, b");
  __asm__("        or  a");
  __asm__("        ret z");
  __asm__("00003$: outi");
  __asm__("        jr  nz, 00003$");
}

void vmem_write_chunk_2(const uint8_t * p, uint16_t n) {
  (void)p;                      // -> HL
  (void)n;                      // -> DE
  __asm__("        ld  c, #0x98");
  __asm__("        ld  a, d");
  __asm__("        or  a");
  __asm__("        jr  z, 00002$");
  __asm__("        ld  b, #0");
  __asm__("00001$: otir");      // 21 or 16 states
  __asm__("        dec d");
  __asm__("        jr  nz, 00001$");
  __asm__("00002$: ld  b, e");
  __asm__("        ld  a, b");
  __asm__("        or  a");
  __asm__("        ret z");
  __asm__("        otir");
}

#include "./vdp_internal.h"

// See also https://www.msx.org/wiki/VRAM_access_speed
void vmem_write_chunk(const uint8_t * p, uint16_t n) {
  // Force Z80 mode to avoid excessive waiting time inserted by S1990.
  // (i.e. Z80 mode is faster than R800 mode in this case!)
  const uint8_t cpu = msx_get_cpu_mode();
  msx_set_cpu_mode(0x00);

  if (sprite_mode < 2) {
    // for MSX1 compatible screen mode and MSX2 TEXT 2 mode.
    // (Requires 28? or 29 cycle for VDP port access.)
    vmem_write_chunk_1(p, n);
  }
  else {
    // for GRAPHIC 3, 4, 5, 6, 7 mode
    vmem_write_chunk_2(p, n);
  }

  msx_set_cpu_mode(cpu);
}

#else

void vmem_write_chunk(const uint8_t * p, uint16_t n) {
  while (n--) {
    vmem_set(*p++);
  }
}

#endif
