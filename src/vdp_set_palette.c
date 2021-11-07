// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_palette.c
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

void vdp_set_palette(uint8_t idx, const palette_t palette) {
  uint8_t* p = (uint8_t*)&palette;
  __critical {
    VDP_SET_CTRL(16, idx);
    vdp_port2 = *p++;           /* r, b */
    vdp_port2 = *p++;           /* g */
  }
}
