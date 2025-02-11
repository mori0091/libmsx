// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_hscroll.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_hscroll(uint16_t x) {
  uint8_t r26 = (x >> 3) & 0x3f;
  uint8_t r27 = (~x + 1) & 0x07;
  if (r27) r26++;
  __asm__("di");
  RG26SA = r26;
  RG27SA = r27;
  VDP_SET_CONTROL_REGISTER(26, r26);
  VDP_SET_CONTROL_REGISTER(27, r27);
  __asm__("ei");
}
