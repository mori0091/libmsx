// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_cmd_execute_LINE.c
 */

#include "../include/vdp.h"

#define diff(a, b)   (((a) <= (b)) ? ((b) - (a)) : ((a) - (b)))

void vdp_cmd_execute_LINE(uint16_t x1, uint16_t y1,
                          uint16_t x2, uint16_t y2,
                          uint8_t color,
                          enum vdp_cmd_logop logop) {
  struct vdp_cmd cmd;

  vdp_cmd_set_DX(&cmd, x1);
  vdp_cmd_set_DY(&cmd, y1);

  const uint16_t w = diff(x1, x2) + 1;
  const uint16_t h = diff(y1, y2) + 1;
  const uint8_t arg = ((y2 < y1) << 3) | ((x2 < x1) << 2);
  if (w <= h) {
    vdp_cmd_set_NX(&cmd, h);
    vdp_cmd_set_NY(&cmd, w);
    vdp_cmd_set_ARG(&cmd, arg | 1);
  }
  else {
    vdp_cmd_set_NX(&cmd, w);
    vdp_cmd_set_NY(&cmd, h);
    vdp_cmd_set_ARG(&cmd, arg | 0);
  }

  vdp_cmd_set_CLR(&cmd, color);
  vdp_cmd_set_logop(&cmd, logop);

  vdp_cmd_execute(&cmd, VDP_CMD_LINE);
}
