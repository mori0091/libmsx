// -*- coding: utf-8-unix -*-

#include "../include/sprite_color.h"

void vmem_set_sprite_color_m(vmemptr_t base, uint8_t plane, uint8_t n,
                             const tagged_color_t* tagged_colors) {
  for (int i = 0; i < n; ++i) {
    vmem_set_sprite_color_s(base, plane + i, tagged_colors[i]);
  }
}
