// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_cmd_execute_POINT.c
 */

#include "../include/vdp.h"

uint8_t vdp_cmd_execute_POINT(uint16_t sx, uint16_t sy) {
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(32);
  __asm__("ei");
  VDP_SET_CONTROL_REGISTER_VALUE(sx & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((sx >> 8) & 1);
  VDP_SET_CONTROL_REGISTER_VALUE(sy & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((sy >> 8) & 3);
  vdp_set_control(46, VDP_CMD_POINT);
  vdp_cmd_await();
  return vdp_get_status(7);
}
