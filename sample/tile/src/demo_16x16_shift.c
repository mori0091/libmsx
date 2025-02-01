// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file demo_16x16_shift.c
 */

#include "./tile.h"

void demo_16x16_shift_operators(void) {
  for (uint8_t i = 0; i <= 32; ++i) {
    uint8_t a = (i < 16 ? i : (32 - i));
    apply_scalar_op_v_SP16x16(a, im_shift_u_v_SP16x16);
  }
  for (uint8_t i = 0; i <= 32; ++i) {
    uint8_t a = (i < 16 ? i : (32 - i));
    apply_scalar_op_v_SP16x16(a, im_shift_d_v_SP16x16);
  }
  for (uint8_t i = 0; i <= 32; ++i) {
    uint8_t a = (i < 16 ? i : (32 - i));
    apply_scalar_op_v_SP16x16(a, im_shift_l_v_SP16x16);
  }
  for (uint8_t i = 0; i <= 32; ++i) {
    uint8_t a = (i < 16 ? i : (32 - i));
    apply_scalar_op_v_SP16x16(a, im_shift_r_v_SP16x16);
  }

  for (uint8_t i = 1; i <= 16; ++i) {
    apply_scalar_op_v_SP16x16(i, im_rotate_u_v_SP16x16);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    apply_scalar_op_v_SP16x16(i, im_rotate_d_v_SP16x16);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    apply_scalar_op_v_SP16x16(i, im_rotate_l_v_SP16x16);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    apply_scalar_op_v_SP16x16(i, im_rotate_r_v_SP16x16);
  }
}

void apply_scalar_op_SP16x16(uint8_t lhs, SP16x16_ScalarOp f) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  SP16x16 * dst = (SP16x16 *)tileset;
  for (int i = 0; i < 64; i++) {
    f(lhs, src++, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
}

void apply_scalar_op_v_SP16x16(uint8_t lhs, SP16x16_ScalarOp_v f) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 64; i++) {
    f(lhs, src++);
  }
}
