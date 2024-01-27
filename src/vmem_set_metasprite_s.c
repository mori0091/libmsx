// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_metasprite_s.c
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
#include "../include/sprite_color.h"
#include "../include/metasprite.h"

void vmem_set_metasprite_s(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms,
                           const tagged_color_t tagged_color) {
  for (int i = 0; i < ms->n; ++i) {
    struct sprite s = {0};
    plane &= 31;
    s.pat = ms->pats[i];
    s.tagged_color = (tagged_color & ~SPRITE_TAG_EC);
    sprite_set_x(&s, ms->layouts[i].x + x - ms->anchor.x);
    sprite_set_y(&s, ms->layouts[i].y + y - ms->anchor.y);
    vmem_write(base + plane * sizeof(struct sprite), &s, sizeof(struct sprite));
    if (2 <= vdp_get_sprite_mode()) {
      vmem_memset(base - 0x0200 + plane * sizeof(struct sprite_color),
                  s.tagged_color, sizeof(struct sprite_color));
    }
    plane++;
  }
}
