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
 * \file demo_16x16_unary.c
 */

#include "./tile.h"

void demo_16x16_unary_operators(void) {
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_hflip_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_vflip_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_tr_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_adtr_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_rot90_v_SP16x16);
  apply_unary_op_v_SP16x16(im_rot180_v_SP16x16);
  apply_unary_op_v_SP16x16(im_rot270_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_rot270_v_SP16x16);
  apply_unary_op_v_SP16x16(im_rot180_v_SP16x16);
  apply_unary_op_v_SP16x16(im_rot90_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);

  apply_unary_op_v_SP16x16(im_cmpl_v_SP16x16);
  apply_unary_op_v_SP16x16(im_copy_v_SP16x16);
}

void apply_unary_op_SP16x16(SP16x16_UnaryOp f) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  SP16x16 * dst = (SP16x16 *)tileset;
  for (int i = 0; i < 64; i++) {
    f(src++, dst++);
  }
  await_vsync();
  copy_tileset_to_VRAM();
  sleep_ticks(15);
}

void apply_unary_op_v_SP16x16(SP16x16_UnaryOp_v f) {
  const SP16x16 * src = (const SP16x16 *)CGTBL;
  await_vsync();
  vmem_set_write_address(PATTERNS);
  for (int i = 0; i < 64; i++) {
    f(src++);
  }
  sleep_ticks(15);
}
