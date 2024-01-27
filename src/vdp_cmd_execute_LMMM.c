// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_execute_LMMM.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

void vdp_cmd_execute_LMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2, enum vdp_cmd_logop logop) {
  uint16_t x1 = x;
  uint16_t y1 = y;
  uint8_t arg = 0;              // VDP_CMD_LRTB

  if (x <= x2) {
    x1 = x + w - 1;
    x2 = x2 + w - 1;
    arg = (1 << 2);             // DIX = 1 (Right to Left)
  }
  if (y <= y2) {
    y1 = y + h - 1;
    y2 = y2 + h - 1;
    arg |= (1 << 3);            // DIY = 1 (Bottom to Top)
  }

  struct vdp_cmd c;
  vdp_cmd_set_SX(&c, x1);
  vdp_cmd_set_SY(&c, y1);
  vdp_cmd_set_DX(&c, x2);
  vdp_cmd_set_DY(&c, y2);
  vdp_cmd_set_NX(&c, w);
  vdp_cmd_set_NY(&c, h);
  // vdp_cmd_set_CLR(&c, 0);
  vdp_cmd_set_ARG(&c, arg);
  vdp_cmd_set_logop(&c, logop);
  vdp_cmd_execute(&c, VDP_CMD_LMMM);
}
