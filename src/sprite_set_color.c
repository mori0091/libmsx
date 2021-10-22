// -*- coding: utf-8-unix -*-

#include "../include/sprite.h"

void sprite_set_color(struct sprite* s, tagged_color_t tagged_color) {
  s->tagged_color &= SPRITE_TAG_EC;
  s->tagged_color |= (tagged_color & ~SPRITE_TAG_EC);
}
