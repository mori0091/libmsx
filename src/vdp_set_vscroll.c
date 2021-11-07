// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_vscroll.c
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

void vdp_set_vscroll(uint8_t y) {
  __critical {
    RG23SA = y;
    VDP_SET_CTRL(23, y);
  }
}
