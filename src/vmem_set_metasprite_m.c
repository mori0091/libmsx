// -*- coding: utf-8-unix -*-

#include "../include/sprite.h"
#include "../include/metasprite.h"

void vmem_set_metasprite_m(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms,
                           const tagged_color_t* tagged_colors) {
  for (int i = 0; i < ms->n; ++i) {
    struct sprite s = {0};
    plane &= 31;
    s.pat = ms->pats[i];
    s.tagged_color = (tagged_colors[i] & ~SPRITE_TAG_EC);
    sprite_set_x(&s, ms->layouts[i].x + x - ms->origin.x);
    sprite_set_y(&s, ms->layouts[i].y + y - ms->origin.y);
    vmem_write(base + plane * sizeof(struct sprite), &s, sizeof(struct sprite));
    if (2 <= vdp_get_sprite_mode()) {
      vmem_memset(base - 0x0200 + plane * sizeof(struct sprite_color),
                  s.tagged_color, sizeof(struct sprite_color));
    }
    plane++;
  }
}
