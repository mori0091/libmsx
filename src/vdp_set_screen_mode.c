// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_set_screen_mode.c
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

struct screen_mode_vec {        // |b7|b6|b5|b4|b3|b2|b1|b0|
  uint8_t r0;                   // | 0| 0| 0| 0|M5|M4|M3| 0|
  uint8_t r1;                   // | 0| 0| 0|M1|M2| 0| 0| 0|
  uint8_t sprite_mode;
};

static const struct screen_mode_vec screen_mode_vecs[] = {
  // `MSX`
  [VDP_SCREEN_MODE_GRAPHIC_1]   = { .r0 = 0x00, .r1 = 0x00, .sprite_mode = 1 }, // SCREEN 1
  [VDP_SCREEN_MODE_TEXT_1]      = { .r0 = 0x00, .r1 = 0x10, .sprite_mode = 0 }, // SCREEN 0, WIDTH 40
  [VDP_SCREEN_MODE_MULTI_COLOR] = { .r0 = 0x00, .r1 = 0x08, .sprite_mode = 1 }, // SCREEN 3
  [VDP_SCREEN_MODE_GRAPHIC_2]   = { .r0 = 0x02, .r1 = 0x00, .sprite_mode = 1 }, // SCREEN 2
  // `MSX2`
  [VDP_SCREEN_MODE_TEXT_2]      = { .r0 = 0x04, .r1 = 0x10, .sprite_mode = 0 }, // SCREEN 0, WIDTH 80
  [VDP_SCREEN_MODE_GRAPHIC_3]   = { .r0 = 0x04, .r1 = 0x00, .sprite_mode = 2 }, // SCREEN 4
  [VDP_SCREEN_MODE_GRAPHIC_4]   = { .r0 = 0x06, .r1 = 0x00, .sprite_mode = 2 }, // SCREEN 5
  [VDP_SCREEN_MODE_GRAPHIC_5]   = { .r0 = 0x08, .r1 = 0x00, .sprite_mode = 2 }, // SCREEN 6
  [VDP_SCREEN_MODE_GRAPHIC_6]   = { .r0 = 0x0a, .r1 = 0x00, .sprite_mode = 2 }, // SCREEN 7
  [VDP_SCREEN_MODE_GRAPHIC_7]   = { .r0 = 0x0e, .r1 = 0x00, .sprite_mode = 2 }, // SCREEN 8, 10, 11, or 12
};

void vdp_set_screen_mode(enum vdp_screen_mode mode) {
  const struct screen_mode_vec * p = &screen_mode_vecs[mode];
  __asm__("di");
  screen_mode = mode;
  sprite_mode = p->sprite_mode;
  RG0SAV = (RG0SAV & ~0x0e) | p->r0;
  RG1SAV = (RG1SAV & ~0x18) | p->r1;
  VDP_SET_CONTROL_REGISTER(0, RG0SAV);
  VDP_SET_CONTROL_REGISTER(1, RG1SAV);
# if (__SDCCCALL == 1)
  // See also https://www.msx.org/wiki/VRAM_access_speed
  if (sprite_mode < 2) {
    // for MSX1 compatible screen mode and MSX2 TEXT 2 mode.
    // (Requires 28? or 29 cycle for VDP port access.)
    vmem__fnptr_write_chunk = vmem_write_chunk_1;
    vmem__fnptr_read_chunk = vmem_read_chunk_1;
  }
  else {
    // for GRAPHIC 3, 4, 5, 6, 7 mode
    vmem__fnptr_write_chunk = vmem_write_chunk_2;
    vmem__fnptr_read_chunk = vmem_read_chunk_2;
  }
# endif
  __asm__("ei");
}
