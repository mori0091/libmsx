// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_palette.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_palette(uint8_t idx, const palette_t palette) {
  uint8_t* p = (uint8_t*)&palette;
  __asm__("di");
  VDP_SET_CONTROL_REGISTER(16, idx);
  __asm__("ei");
  vdp_port2 = *p++;             /* r, b */
  vdp_port2 = *p++;             /* g */
}
