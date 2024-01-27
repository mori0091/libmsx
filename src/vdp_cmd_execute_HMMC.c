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
 * \file vdp_cmd_execute_HMMC.c
 */

#include "../include/vdp.h"

extern uint8_t vdp_cmd_write_CMD;

void vdp_cmd_execute_HMMC(uint16_t x, uint16_t y,
                          uint16_t w, uint16_t h,
                          enum vdp_cmd_dir dir) {
  struct vdp_cmd cmd;
  vdp_cmd_set_DX(&cmd, x);
  vdp_cmd_set_DY(&cmd, y);
  vdp_cmd_set_NX(&cmd, w);
  vdp_cmd_set_NY(&cmd, h);
  // vdp_cmd_set_CLR(&cmd, 0);
  vdp_cmd_set_ARG(&cmd, dir);

  // Don't execute at here, just setup for the 1st call to vdp_cmd_write().
  vdp_cmd_write_CMD = VDP_CMD_HMMC;
  vdp_cmd_await();
  vdp_write_control(36, &cmd.r36, 10);
}
