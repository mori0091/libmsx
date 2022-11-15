// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_set_unrestricted.c
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

void vdp_cmd_set_unrestricted(bool enable) {
  __asm__("di");
  if (enable) {
    RG25SA |= 0x40;
  } else {
    RG25SA &= ~0x40;
  }
  VDP_SET_CONTROL_REGISTER(25, RG25SA);
  __asm__("ei");
}
