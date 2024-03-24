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
 * \file im_tilemap_v_BM8x8.c
 */

#include <im.h>

void im_tilemap_v_BM8x8(const BM8x8 tileset[256], size_t n, const uint8_t tilemap[][2]) {
  const uint8_t * p = tilemap[0];
  while (n--) {
    const BM8x8 * src = &tileset[*p++];
     switch (*p++) {
      case IM_COPY:
        im_copy_v_BM8x8(src);
        break;
      case IM_FLIP_HORZ:
        im_hflip_v_BM8x8(src);
        break;
      case IM_FLIP_VERT:
        im_vflip_v_BM8x8(src);
        break;
      case IM_TRANSPOSE:
        im_tr_v_BM8x8(src);
        break;
      case IM_ROTATE_90:
        im_rot90_v_BM8x8(src);
        break;
      case IM_ROTATE_180:
        im_rot180_v_BM8x8(src);
        break;
      case IM_ROTATE_270:
        im_rot270_v_BM8x8(src);
        break;
      case IM_ANTITRANSPOSE:
        im_adtr_v_BM8x8(src);
        break;
      case IM_COMPLEMENT:
        im_cmpl_v_BM8x8(src);
        break;
    }
  }
}
