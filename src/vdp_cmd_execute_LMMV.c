// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_execute_LMMV.c
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

void vdp_cmd_execute_LMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color, enum vdp_cmd_logop logop) {
  struct vdp_cmd c;
  vdp_cmd_set_DX(&c, x);
  vdp_cmd_set_DY(&c, y);
  vdp_cmd_set_NX(&c, w);
  vdp_cmd_set_NY(&c, h);
  vdp_cmd_set_CLR(&c, color);
  vdp_cmd_set_ARG(&c, 0);
  vdp_cmd_set_logop(&c, logop);
  vdp_cmd_execute(&c, VDP_CMD_LMMV);
}
