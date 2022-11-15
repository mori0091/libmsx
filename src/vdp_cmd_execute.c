// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_execute.c
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

static void vdp_cmd_exec_r32(const struct vdp_cmd * c, enum vdp_cmd_op opcode) {
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(32);
  __asm__("ei");
  const uint8_t * p = &c->r32;
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p | opcode);
}

static void vdp_cmd_exec_r36(const struct vdp_cmd * c, enum vdp_cmd_op opcode) {
  vdp_cmd_await();
  __asm__("di");
  VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(36);
  __asm__("ei");
  const uint8_t * p = &c->r36;
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p++);
  VDP_SET_CONTROL_REGISTER_VALUE(*p | opcode);
}

void vdp_cmd_execute(const struct vdp_cmd * c, enum vdp_cmd_op opcode) {
  switch (opcode) {
  case VDP_CMD_LMMV:
    vdp_cmd_exec_r36(c, opcode);
    return;
  case VDP_CMD_LMMM:
    vdp_cmd_exec_r32(c, opcode);
    return;
  case VDP_CMD_HMMV:
    vdp_cmd_exec_r36(c, opcode);
    return;
  case VDP_CMD_HMMM:
    vdp_cmd_exec_r32(c, opcode);
    return;
  default:
    return;
  }
}
