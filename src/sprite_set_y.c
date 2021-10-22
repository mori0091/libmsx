// -*- coding: utf-8-unix -*-

#include "../include/sprite.h"

void sprite_set_y(struct sprite* s, int y) {
  s->y = (uint8_t)(y & 0xFF);
}
