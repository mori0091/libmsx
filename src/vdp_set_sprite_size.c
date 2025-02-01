// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_sprite_size.c
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

void vdp_set_sprite_size(enum vdp_sprite_size size) {
  __asm__("di");
  RG1SAV = (RG1SAV & ~0x03) | size;
  VDP_SET_CONTROL_REGISTER(1, RG1SAV);
  __asm__("ei");
}
