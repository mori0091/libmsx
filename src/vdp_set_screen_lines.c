// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_screen_lines.c
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

void vdp_set_screen_lines(enum vdp_screen_lines lines) {
  __asm__("di");
  RG9SAV = (RG9SAV & ~0x80) | lines;
  VDP_SET_CONTROL_REGISTER(9, RG9SAV);
  __asm__("ei");
}
