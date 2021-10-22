// -*- coding: utf-8-unix -*-

#include "../include/sprite.h"

void sprite_set_xy(struct sprite* s, int x, int y) {
  sprite_set_y(s, y);
  sprite_set_x(s, x);
}
