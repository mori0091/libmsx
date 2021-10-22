// -*- coding: utf-8-unix -*-

#include "../include/sprite.h"
#include "../include/metasprite.h"

void vmem_set_metasprite_a(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms) {
  for (int i = 0; i < ms->n; ++i) {
    struct sprite s = {0};
    plane &= 31;
    s.pat = ms->pats[i];
    sprite_set_x(&s, ms->layouts[i].x + x - ms->origin.x);
    sprite_set_y(&s, ms->layouts[i].y + y - ms->origin.y);
    vmem_write(base + plane * sizeof(struct sprite), &s, 3);
    plane++;
  }
}
