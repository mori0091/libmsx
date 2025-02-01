// -*- coding: utf-8-unix -*-
/**
 * \file sprite_set_y.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/sprite.h"

void sprite_set_y(struct sprite* s, int y) {
  s->y = (uint8_t)(y & 0xFF);
}
