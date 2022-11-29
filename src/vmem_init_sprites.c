// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vmem_init_sprites.c
 */

#include <vmem.h>

void vmem_init_sprites(const vmemptr_t base) {
  vmem_set_write_address(base);
  for (uint8_t n = 0; n < 32; n++) {
    vmem_set(217);              // y
    vmem_set(0);                // x
    vmem_set(n);                // pat
    vmem_set(0);                // tag | color
  }
}
