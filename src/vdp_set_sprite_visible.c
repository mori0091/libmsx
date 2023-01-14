// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_sprite_visible.c
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

void vdp_set_sprite_visible(bool visible) {
  __asm__("di");
  if (visible) {
    RG8SAV &= ~0x02;
  } else {
    RG8SAV |= 0x02;
  }
  VDP_SET_CONTROL_REGISTER(8, RG8SAV);
  __asm__("ei");
}
