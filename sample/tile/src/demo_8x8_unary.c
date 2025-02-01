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
 * \file demo_8x8_unary.c
 */

#include "./tile.h"

void demo_8x8_unary_operators(void) {
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_hflip_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_vflip_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_tr_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_adtr_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_rot90_v_BM8x8);
  apply_unary_op_v_BM8x8(im_rot180_v_BM8x8);
  apply_unary_op_v_BM8x8(im_rot270_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_rot270_v_BM8x8);
  apply_unary_op_v_BM8x8(im_rot180_v_BM8x8);
  apply_unary_op_v_BM8x8(im_rot90_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);

  apply_unary_op_v_BM8x8(im_cmpl_v_BM8x8);
  apply_unary_op_v_BM8x8(im_copy_v_BM8x8);
}

void apply_unary_op_BM8x8(BM8x8_UnaryOp f) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  BM8x8 * dst = tileset;
  for (int i = 0; i < 256; i++) {
    f(src++, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
  sleep_ticks(15);
}

void apply_unary_op_v_BM8x8(BM8x8_UnaryOp_v f) {
  const BM8x8 * src = (const BM8x8 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 256; i++) {
    f(src++);
  }
  sleep_ticks(15);
}
