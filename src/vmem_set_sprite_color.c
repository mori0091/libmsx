// -*- coding: utf-8-unix -*-

#include "../include/sprite_color.h"

void vmem_set_sprite_color(vmemptr_t base, uint8_t plane,
                           const struct sprite_color* sprite_color) {
  vmem_write(base - 0x0200 + plane * sizeof(struct sprite_color),
             sprite_color, sizeof(struct sprite_color));
}
