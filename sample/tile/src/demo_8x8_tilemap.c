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

void demo_8x8_tilemap_operator(void) {
  copy_tileset_from_MainROM();
  copy_tileset_to_VRAM();
  const BM8x8 * rom_font_tileset = (const BM8x8 *)CGTBL;
  BM8x8 * q = tileset;
  for (int i = 0; i < 256; ++i) {
    uint8_t tiles[][4][2] = {
      {
        [0] = {i, IM_FLIP_HORZ}, [2] = {i, IM_COPY},
        [1] = {i, IM_ROTATE_180},[3] = {i, IM_FLIP_VERT}
      },
      {
        [0] = {i, IM_COPY},      [2] = {i, IM_FLIP_HORZ},
        [1] = {i, IM_FLIP_VERT}, [3] = {i, IM_ROTATE_180}
      },
      {
        [0] = {i, IM_COPY},      [2] = {i, IM_ROTATE_90},
        [1] = {i, IM_ROTATE_270},[3] = {i, IM_ROTATE_180}
      },
      {
        [0] = {i, IM_TRANSPOSE}, [2] = {i, IM_ANTITRANSPOSE},
        [1] = {i, IM_ROTATE_270},[3] = {i, IM_ROTATE_90}
      },
      {
        [0] = {(i+0)&255, IM_FLIP_HORZ}, [2] = {(i+1)&255, IM_COPY},
        [1] = {(i+2)&255, IM_ROTATE_180},[3] = {(i+3)&255, IM_FLIP_VERT}
      },
      {
        [0] = {(i+0)&255, IM_COPY},      [2] = {(i+1)&255, IM_FLIP_HORZ},
        [1] = {(i+2)&255, IM_FLIP_VERT}, [3] = {(i+3)&255, IM_ROTATE_180}
      },
      {
        [0] = {(i+0)&255, IM_COPY},      [2] = {(i+1)&255, IM_ROTATE_90},
        [1] = {(i+2)&255, IM_ROTATE_270},[3] = {(i+3)&255, IM_ROTATE_180}
      },
      {
        [0] = {(i+0)&255, IM_TRANSPOSE}, [2] = {(i+1)&255, IM_ANTITRANSPOSE},
        [1] = {(i+2)&255, IM_ROTATE_270},[3] = {(i+3)&255, IM_ROTATE_90}
      },
    };
    await_vsync();
    vmem_set_write_address(PATTERNS);
    for (uint8_t n = 3; n--; ) {
      im_tilemap_v_BM8x8(rom_font_tileset, 32, tiles[0]);
    }
  }
}
