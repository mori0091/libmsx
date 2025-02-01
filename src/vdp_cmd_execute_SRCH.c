// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_cmd_execute_SRCH.c
 */

#include "../include/vdp.h"

bool vdp_cmd_execute_SRCH(uint16_t * ret, uint16_t sx, uint16_t sy, uint8_t color, uint8_t arg) {
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(32);
  __asm__("ei");
  VDP_SET_CONTROL_REGISTER_VALUE(sx & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((sx >> 8) & 1);
  VDP_SET_CONTROL_REGISTER_VALUE(sy & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((sy >> 8) & 3);

  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(44);
  __asm__("ei");
  VDP_SET_CONTROL_REGISTER_VALUE(color);
  VDP_SET_CONTROL_REGISTER_VALUE(arg);
  VDP_SET_CONTROL_REGISTER_VALUE(VDP_CMD_SRCH);

  uint8_t s2;
  do {
    s2 = vdp_get_status(2);
  } while (s2 & 1);

  if (!(s2 & 0x10)) {
    // not found
    return false;
  }

  if (ret) {
    *ret = (vdp_get_status(9) & 3) << 8 | vdp_get_status(8);
  }
  return true;
}
