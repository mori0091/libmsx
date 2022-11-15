// -*- coding: utf-8-unix -*-
/**
 * \file vdp_get_status.c
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

uint8_t vdp_get_status(uint8_t reg) {
  volatile uint8_t x;
  __asm__("di");
  VDP_SET_STATUS_REGISTER_POINTER(reg);
  x = VDP_GET_STATUS_REGISTER_VALUE();
  VDP_SET_STATUS_REGISTER_POINTER(0);
  __asm__("ei");
  return x;
}
