// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_sprite_color_s.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"
#include "../include/vmem.h"
#include "../include/sprite_color.h"

void vmem_set_sprite_color_s(vmemptr_t base, uint8_t plane,
                             const tagged_color_t tagged_color) {
  const int m = vdp_get_sprite_mode();
  if (!m) return;
  if (m == 1) {
    vmem_set_write_address(base + plane * sizeof(struct sprite) + 3);
    vmem_set(tagged_color);
    return;
  }
  vmem_memset(base - 0x0200 + plane * sizeof(struct sprite_color),
              tagged_color, sizeof(struct sprite_color));
}
