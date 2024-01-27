// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_cmd_execute_PSET.c
 */

#include "../include/vdp.h"

void vdp_cmd_execute_PSET(uint16_t dx, uint16_t dy, uint8_t color, enum vdp_cmd_logop logop) {
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(36);
  __asm__("ei");
  VDP_SET_CONTROL_REGISTER_VALUE(dx & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((dx >> 8) & 1);
  VDP_SET_CONTROL_REGISTER_VALUE(dy & 255);
  VDP_SET_CONTROL_REGISTER_VALUE((dy >> 8) & 3);

  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(44);
  __asm__("ei");
  VDP_SET_CONTROL_REGISTER_VALUE(color);
  VDP_SET_CONTROL_REGISTER_VALUE(0);
  VDP_SET_CONTROL_REGISTER_VALUE(VDP_CMD_PSET | logop);
}
