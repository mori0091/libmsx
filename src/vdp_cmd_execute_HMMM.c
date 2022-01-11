// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_execute_HMMM.c
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

void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2) {
  struct vdp_cmd c;
  vdp_cmd_set_SX(&c, x);
  vdp_cmd_set_SY(&c, y);
  vdp_cmd_set_DX(&c, x2);
  vdp_cmd_set_DY(&c, y2);
  vdp_cmd_set_NX(&c, w);
  vdp_cmd_set_NY(&c, h);
  vdp_cmd_set_CLR(&c, 0);
  vdp_cmd_set_ARG(&c, 0);
  vdp_cmd_set_logop(&c, 0);
  vdp_cmd_execute(&c, VDP_CMD_HMMM);
}
