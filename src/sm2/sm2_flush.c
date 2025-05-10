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
 * \file sm2_flush.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

void sm2_flush(void) {
  uint8_t n = sm2__num_planes_in_use - sm2__num_planes_reserved;
  if (sm2__num_planes_in_use < SM2_SPRITE_PLANE_MAX) {
    sm2__sprite_table.sat[sm2__num_planes_in_use].y = 216;
    n++;
  }
  if (n) {
    sm2__flush(sm2__num_planes_reserved, n);
  }
  // ---- clear list
  sm2__num_planes_in_use = sm2__num_planes_reserved;
  // ---- clear pattern cache
  if (sm2__cache_missed) {
    sm2__cache_missed = false;
    sm2__LiveCel_clear_all();
  }
}
