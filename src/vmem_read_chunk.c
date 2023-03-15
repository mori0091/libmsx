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
 * \file vmem_read_chunk.c
 */

#include "../include/vdp.h"

#if (__SDCCCALL == 1)

#include "./vdp_internal.h"

void vmem_read_chunk_1(uint8_t * p, uint16_t n) {
  (void)p;                      // -> HL
  (void)n;                      // -> DE
  __asm__("        ld  c, #0x98");
  __asm__("        ld  a, d");
  __asm__("        or  a");
  __asm__("        jr  z, 00002$");
  __asm__("        ld  b, #0");
  __asm__("00001$: ini");            // 16 states
  __asm__("        jr  nz, 00001$"); // 12 or 7 states
  __asm__("        dec d");
  __asm__("        jr  nz, 00001$");
  __asm__("00002$: ld  b, e");
  __asm__("        ld  a, b");
  __asm__("        or  a");
  __asm__("        ret z");
  __asm__("00003$: ini");
  __asm__("        jr  nz, 00003$");
}

void vmem_read_chunk_2(uint8_t * p, uint16_t n) {
  (void)p;                      // -> HL
  (void)n;                      // -> DE
  __asm__("        ld  c, #0x98");
  __asm__("        ld  a, d");
  __asm__("        or  a");
  __asm__("        jr  z, 00002$");
  __asm__("        ld  b, #0");
  __asm__("00001$: inir");      // 21 or 16 states
  __asm__("        dec d");
  __asm__("        jr  nz, 00001$");
  __asm__("00002$: ld  b, e");
  __asm__("        ld  a, b");
  __asm__("        or  a");
  __asm__("        ret z");
  __asm__("        inir");
}

void (* vmem__fnptr_read_chunk)(uint8_t * p, uint16_t n) = vmem_read_chunk_1;

// See also https://www.msx.org/wiki/VRAM_access_speed
void vmem_read_chunk(uint8_t * p, uint16_t n) {
  vmem__fnptr_read_chunk(p, n);
}

#else

void vmem_read_chunk(uint8_t * p, uint16_t n) {
  while (n--) {
    *p++ = vmem_get();
  }
}

#endif
