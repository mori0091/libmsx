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
 * \file sm2_reserve.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

static void sm2__hide(uint8_t base_plane, uint8_t n);

void sm2_reserve(uint8_t n) {
  if (SM2_SPRITE_PLANE_MAX < n) {
    n = SM2_SPRITE_PLANE_MAX;
  }
  sm2__num_planes_reserved = n;
  const int8_t m = n - sm2__num_planes_in_use;
  if (0 < m) {
    sm2__hide(sm2__num_planes_in_use, m);
    sm2__num_planes_in_use = n;
  }
}

#include <string.h>

static void sm2__hide(uint8_t base_plane, uint8_t n) {
  uint8_t * sat = (uint8_t *)&sm2__sprite_table.sat[base_plane];
  uint8_t * sct = (uint8_t *)&sm2__sprite_table.sct[base_plane];
  memset(sat, 0, n * sizeof(struct sprite));
  memset(sct, (SPRITE_TAG_CC | SPRITE_TAG_IC | SPRITE_TAG_EC), n * sizeof(struct sprite_color));
  sm2__flush(base_plane, n);
}
