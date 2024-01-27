// -*- coding: utf-8-unix -*-
/**
 * \file vdp_write_control.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_write_control(uint8_t reg, void* src, uint8_t len) {
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(reg);
  for (uint8_t* p = src; len--; ) {
    VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  }
  __asm__("ei");
}
