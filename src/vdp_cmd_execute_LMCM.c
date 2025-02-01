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
 * \file vdp_cmd_execute_LMCM.c
 */

#include "../include/vdp.h"

void vdp_cmd_execute_LMCM(uint16_t x, uint16_t y,
                          uint16_t w, uint16_t h,
                          enum vdp_cmd_dir dir) {
  struct vdp_cmd cmd;
  vdp_cmd_set_SX(&cmd, x);
  vdp_cmd_set_SY(&cmd, y);
  vdp_cmd_set_NX(&cmd, w);
  vdp_cmd_set_NY(&cmd, h);
  vdp_cmd_set_ARG(&cmd, dir);
  vdp_get_status(7);            // clear TR bit
  vdp_cmd_execute(&cmd, VDP_CMD_LMCM);
}
