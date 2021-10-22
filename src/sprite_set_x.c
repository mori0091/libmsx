// -*- coding: utf-8-unix -*-

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
