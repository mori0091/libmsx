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
 * \file demo_16x16_binary.c
 */

#include "./tile.h"

void demo_16x16_binary_operators(void) {
  for (uint8_t i = 0; i < 9; ++i) {
    uint8_t a = (i < 4 ? (4 - i) : (i - 4));
    apply_binary_op_v_SP16x16(im_diff_v_SP16x16, &sprite_masks[a]);
  }
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  for (uint8_t i = 0; i < 9; ++i) {
    uint8_t a = (i < 4 ? (4 - i) : (i - 4));
    apply_binary_op_v_SP16x16(im_and_v_SP16x16, &sprite_masks[a]);
  }
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  for (uint8_t i = 0; i < 9; ++i) {
    uint8_t a = (i < 4 ? (4 - i) : (i - 4));
    apply_binary_op_v_SP16x16(im_or_v_SP16x16, &sprite_masks[a]);
  }
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  for (uint8_t i = 0; i < 9; ++i) {
    uint8_t a = (i < 4 ? (4 - i) : (i - 4));
    apply_binary_op_v_SP16x16(im_xor_v_SP16x16, &sprite_masks[a]);
  }
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);
}

void apply_binary_op_SP16x16(SP16x16_BinaryOp f, const SP16x16 * rhs) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  SP16x16 * dst = (SP16x16 *)tileset;
  for (int i = 0; i < 64; i++) {
    f(src++, rhs, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
}

void apply_binary_op_v_SP16x16(SP16x16_BinaryOp_v f, const SP16x16 * rhs) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 64; i++) {
    f(src++, rhs);
  }
}
