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
 * \file demo_8x8_binary.c
 */

#include "./tile.h"

void demo_8x8_binary_operators(void) {
  for (uint8_t i = 0; i <= 10; ++i) {
    uint8_t a = (i < 5 ? (5 - i) : (i - 5));
    apply_binary_op_v_BM8x8(im_diff_v_BM8x8, &masks[a]);
  }
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  for (uint8_t i = 0; i <= 10; ++i) {
    uint8_t a = (i < 5 ? (5 - i) : (i - 5));
    apply_binary_op_v_BM8x8(im_and_v_BM8x8, &masks[a]);
  }
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  for (uint8_t i = 0; i <= 10; ++i) {
    uint8_t a = (i < 5 ? (5 - i) : (i - 5));
    apply_binary_op_v_BM8x8(im_or_v_BM8x8, &masks[a]);
  }
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  for (uint8_t i = 0; i <= 10; ++i) {
    uint8_t a = (i < 5 ? (5 - i) : (i - 5));
    apply_binary_op_v_BM8x8(im_xor_v_BM8x8, &masks[a]);
  }
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);
}

void apply_binary_op_BM8x8(BM8x8_BinaryOp f, const BM8x8 * rhs) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  BM8x8 * dst = tileset;
  for (int i = 0; i < 256; i++) {
    f(src++, rhs, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
}

void apply_binary_op_v_BM8x8(BM8x8_BinaryOp_v f, const BM8x8 * rhs) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 256; i++) {
    f(src++, rhs);
  }
}
