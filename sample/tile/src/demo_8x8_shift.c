// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file demo_8x8_shift.c
 */

#include "./tile.h"

void demo_8x8_shift_operators(void) {
  for (uint8_t i = 1; i <= 16; ++i) {
    uint8_t a = (i < 8 ? i : (16 - i));
    apply_scalar_op_v_BM8x8(a, im_shift_u_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    uint8_t a = (i < 8 ? i : (16 - i));
    apply_scalar_op_v_BM8x8(a, im_shift_d_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    uint8_t a = (i < 8 ? i : (16 - i));
    apply_scalar_op_v_BM8x8(a, im_shift_l_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 16; ++i) {
    uint8_t a = (i < 8 ? i : (16 - i));
    apply_scalar_op_v_BM8x8(a, im_shift_r_v_BM8x8);
  }

  for (uint8_t i = 1; i <= 8; ++i) {
    apply_scalar_op_v_BM8x8(i, im_rotate_u_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 8; ++i) {
    apply_scalar_op_v_BM8x8(i, im_rotate_d_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 8; ++i) {
    apply_scalar_op_v_BM8x8(i, im_rotate_l_v_BM8x8);
  }
  for (uint8_t i = 1; i <= 8; ++i) {
    apply_scalar_op_v_BM8x8(i, im_rotate_r_v_BM8x8);
  }
}

void apply_scalar_op_BM8x8(uint8_t lhs, BM8x8_ScalarOp f) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  BM8x8 * dst = tileset;
  for (int i = 0; i < 256; i++) {
    f(lhs, src++, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
}

void apply_scalar_op_v_BM8x8(uint8_t lhs, BM8x8_ScalarOp_v f) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 256; i++) {
    f(lhs, src++);
  }
}
