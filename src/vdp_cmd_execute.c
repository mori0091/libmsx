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
 * \file vdp_cmd_execute.c
 */

#include "../include/vdp.h"

static const uint8_t vdp_cmd_vec[] = {
  [VDP_CMD_STOP  >> 4] = 46, // 0000b STOP
  [1]                  = 46, // 0001b invalid
  [2]                  = 46, // 0010b invalid
  [3]                  = 46, // 0011b invalid
  [VDP_CMD_POINT >> 4] = 32, // 0100b POINT
  [VDP_CMD_PSET  >> 4] = 36, // 0101b PSET
  [VDP_CMD_SRCH  >> 4] = 32, // 0110b SRCH
  [VDP_CMD_LINE  >> 4] = 36, // 0111b LINE
  [VDP_CMD_LMMV  >> 4] = 36, // 1000b LMMV
  [VDP_CMD_LMMM  >> 4] = 32, // 1001b LMMM
  [VDP_CMD_LMCM  >> 4] = 32, // 1010b LMCM
  [VDP_CMD_LMMC  >> 4] = 36, // 1011b LMMC
  [VDP_CMD_HMMV  >> 4] = 36, // 1100b HMMV
  [VDP_CMD_HMMM  >> 4] = 32, // 1101b HMMM
  [VDP_CMD_YMMM  >> 4] = 34, // 1110b YMMM (r34..r39, r42..r43, r45..r46)
  [VDP_CMD_HMMC  >> 4] = 36, // 1111b HMMC
};

void vdp_cmd_execute(const struct vdp_cmd * c, enum vdp_cmd_op opcode) {
  uint8_t reg = vdp_cmd_vec[opcode >> 4];
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(reg);
  __asm__("ei");
  const uint8_t * p = &c->r32 + reg - 32;
  for (uint8_t n = 46 - reg; n--; ) {
    VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  }
  VDP_SET_CONTROL_REGISTER_VALUE(*p | opcode);
}
