// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_control.c
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

void vdp_set_control(uint8_t reg, uint8_t x) {
  __asm__("di");
  VDP_SET_CONTROL_REGISTER(reg, x);
  __asm__("ei");
}
