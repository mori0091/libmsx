// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_hscroll_mask.c
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

void vdp_set_hscroll_mask(bool enable) {
  __critical {
    if (enable) {
      RG25SA |= 0x02;
    } else {
      RG25SA &= ~0x02;
    }
    VDP_SET_CTRL(25, RG25SA);
  }
}
