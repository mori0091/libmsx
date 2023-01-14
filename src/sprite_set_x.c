// -*- coding: utf-8-unix -*-
/**
 * \file sprite_set_x.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/sprite.h"

void sprite_set_x(struct sprite* s, int x) {
  if (x < -32) x = -32;
  if (x > 255) x = 255;
  if (x < 0) {
    s->x = x + 32;
    s->tagged_color |= SPRITE_TAG_EC;
  } else {
    s->x = x;
    s->tagged_color &= ~SPRITE_TAG_EC;
  }
}
