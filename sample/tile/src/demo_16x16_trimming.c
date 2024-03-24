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
 * \file demo_8x8_tilemap.c
 */

#include "./tile.h"

void demo_16x16_trimming_operator(void) {
  for (uint8_t n = 4; n--; ) {
    copy_tileset_from_MainROM();
    for (uint8_t i = 0; i <= 32; ++i) {
      uint8_t a = (i < 16 ? i : (32 - i));
      uint8_t tblr[][4] = {
      {a, 0, 0, 0}, // Trim top of the sprite pattern
      {0, a, 0, 0}, // Trim bottom of the sprite pattern
      {0, 0, a, 0}, // Trim left of the sprite pattern
      {0, 0, 0, a}, // Trim right of the sprite pattern

      {a, 0, a, 0}, // Trim top and left of the sprite pattern
      {a, 0, 0, a}, // Trim top and right of the sprite pattern
      {0, a, a, 0}, // Trim bottom and left of the sprite pattern
      {0, a, 0, a}, // Trim bottom and right of the sprite pattern

      {a/2, a/2, 0, 0}, // Trim top and bottom of the sprite pattern
      {0, 0, a/2, a/2}, // Trim left and right of the sprite pattern
      {a*1/4, a*3/4, 0, 0}, // Trim top and bottom of the sprite pattern
      {a*1/4, a*3/4, a*1/2, a*1/2},
    };
      im_trim_SP16x16(tblr[0], (const SP16x16 *)&CGTBL->data[0], (SP16x16 *)&tileset[0]);
      im_trim_SP16x16(tblr[1], (const SP16x16 *)&CGTBL->data[4], (SP16x16 *)&tileset[4]);
      im_trim_SP16x16(tblr[2], (const SP16x16 *)&CGTBL->data[8], (SP16x16 *)&tileset[8]);
      im_trim_SP16x16(tblr[3], (const SP16x16 *)&CGTBL->data[12], (SP16x16 *)&tileset[12]);

      im_trim_SP16x16(tblr[4], (const SP16x16 *)&CGTBL->data[32], (SP16x16 *)&tileset[32]);
      im_trim_SP16x16(tblr[5], (const SP16x16 *)&CGTBL->data[36], (SP16x16 *)&tileset[36]);
      im_trim_SP16x16(tblr[6], (const SP16x16 *)&CGTBL->data[40], (SP16x16 *)&tileset[40]);
      im_trim_SP16x16(tblr[7], (const SP16x16 *)&CGTBL->data[44], (SP16x16 *)&tileset[44]);

      im_trim_SP16x16(tblr[8], (const SP16x16 *)&CGTBL->data[48], (SP16x16 *)&tileset[48]);
      im_trim_SP16x16(tblr[9], (const SP16x16 *)&CGTBL->data[56], (SP16x16 *)&tileset[56]);
      im_trim_SP16x16(tblr[10], (const SP16x16 *)&CGTBL->data[64], (SP16x16 *)&tileset[64]);
      im_trim_SP16x16(tblr[11], (const SP16x16 *)&CGTBL->data[68], (SP16x16 *)&tileset[68]);

      await_vsync();
      copy_tileset_to_VRAM();
    }
  }
}
