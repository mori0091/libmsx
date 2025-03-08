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
 * \file sm2_add_cel.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

void sm2_add_cel(const sm2_Cel * cel, int x, int y) {
  const size_t a0 = sm2__num_planes_in_use + cel->depth;
  if (a0 > SM2_SPRITE_PLANE_MAX) {
    sm2_flush();
  }
  const size_t a = sm2__num_planes_in_use + cel->depth;
  if (a <= SM2_SPRITE_PLANE_MAX) {
    if (sm2__put_cel(sm2__num_planes_in_use, cel, x, y)) {
      sm2__num_planes_in_use = a;
    }
  }
}
