// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_adjust.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_adjust(int8_t x, int8_t y) {
  if (x < -7) x = -7;
  if (y < -7) y = -7;
  if (8 < x) x = 8;
  if (8 < y) y = 8;
  x = (0 < x) ? 16 - x : -x;
  y = (0 < y) ? 16 - y : -y;
  __asm__("di");
  RG18SA = (y << 4) | x;
  VDP_SET_CONTROL_REGISTER(18, RG18SA);
  __asm__("ei");
}
