// -*- coding: utf-8-unix -*-
/**
 * \file sprite_set_xy.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/sprite.h"

void sprite_set_xy(struct sprite* s, int x, int y) {
  sprite_set_y(s, y);
  sprite_set_x(s, x);
}
