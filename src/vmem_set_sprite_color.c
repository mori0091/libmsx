// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_sprite_color.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/sprite_color.h"

void vmem_set_sprite_color(vmemptr_t base, uint8_t plane,
                           const struct sprite_color* sprite_color) {
  vmem_write(base - 0x0200 + plane * sizeof(struct sprite_color),
             sprite_color, sizeof(struct sprite_color));
}
