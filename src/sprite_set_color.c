// -*- coding: utf-8-unix -*-
/**
 * \file sprite_set_color.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/sprite.h"

void sprite_set_color(struct sprite* s, tagged_color_t tagged_color) {
  s->tagged_color &= SPRITE_TAG_EC;
  s->tagged_color |= (tagged_color & ~SPRITE_TAG_EC);
}
