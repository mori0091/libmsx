// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_metasprite_a.c
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
#include "../include/metasprite.h"

void vmem_set_metasprite_a(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms) {
  for (int i = 0; i < ms->n; ++i) {
    struct sprite s = {0};
    plane &= 31;
    s.pat = ms->pats[i];
    sprite_set_x(&s, ms->layouts[i].x + x - ms->anchor.x);
    sprite_set_y(&s, ms->layouts[i].y + y - ms->anchor.y);
    if (s.tagged_color & SPRITE_TAG_EC) {
      // if there is a sprite whose x-coordinate would to be less than 0 on the
      // screen, that sprite is hidden instead of setting the EC bit.
      s.y = 217;
    }
    vmem_write(base + plane * sizeof(struct sprite), &s, 3);
    plane++;
  }
}
