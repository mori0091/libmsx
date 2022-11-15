// -*- coding: utf-8-unix -*-
/**
 * \file vdp_write_palette.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_write_palette(const palette_t palettes[16]) {
  const uint8_t* p = (const uint8_t*)palettes;
  __asm__("di");
  VDP_SET_CONTROL_REGISTER(16, 0);
  __asm__("ei");
  for (uint8_t i = 16; i--; ) {
    vdp_port2 = *p++;  /* R, B */
    vdp_port2 = *p++;  /* G */
  }
}
