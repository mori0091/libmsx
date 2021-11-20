// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_sprite_size.c
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

void vdp_set_sprite_size(enum sprite_size size) {
  __critical {
    RG1SAV = (RG1SAV & ~0x03) | size;
    VDP_SET_CONTROL_REGISTER(1, RG1SAV);
  }
}
