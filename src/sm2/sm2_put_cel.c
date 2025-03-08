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
 * \file sm2_put_cel.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

void sm2_put_cel(uint8_t base_plane, const sm2_Cel * cel, int x, int y) {
  const size_t a = base_plane + cel->depth;
  if (a <= sm2__num_planes_reserved) {
    if (sm2__put_cel(base_plane, cel, x, y)) {
      sm2__flush(base_plane, cel->depth);
    }
  }
}
