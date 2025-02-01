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
 * \file demo_8x8_tilemap.c
 */

#include "./tile.h"

void demo_8x8_vec_shift_operators(void) {
  copy_tileset_from_MainROM();
  for (uint8_t m = 64; m--; ) {
    BM8x8 * row = tileset;
    for (uint8_t n = 8; n--; ) {
      im_rotate_r_vec_BM8x8(1, 8, row, row); row += 8;
      im_shift_l_vec_BM8x8( 1, 8, row, row); row += 8;
      im_shift_r_vec_BM8x8( 1, 8, row, row); row += 8;
      im_rotate_l_vec_BM8x8(1, 8, row, row); row += 8;
    }
    await_vsync();
    copy_tileset_to_VRAM();
  }
}
