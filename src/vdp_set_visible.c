// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_visible.c
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

void vdp_set_visible(bool visible) {
  __asm__("di");
  if (visible) {
    RG1SAV |= 0x40;
  } else {
    RG1SAV &= ~0x40;
  }
  VDP_SET_CONTROL_REGISTER(1, RG1SAV);
  __asm__("ei");
}
