// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file sm2_bmem.c
 */

#include "sm2_bmem.h"

#include <resources.h>

inline size_t sm2__cel_sizeof(uint8_t depth) { return 1 + (32 + 16 + 4) * depth; }

size_t sm2_load_cel_resource(const char * name, sm2_Cel cels[], size_t cels_max) {
  const ResourceIndex * r = resource_find(name);
  if (!r) return 0;
  bmemptr_t addr = r->offset;
  const bmemptr_t end = addr + r->size;
  size_t n = 0;
  while (n < cels_max && addr < end) {
    const uint8_t depth = bmem_get(addr);
    cels->addr = addr;
    cels->depth = depth;
    addr += sm2__cel_sizeof(depth);
    cels++;
    n++;
  }
  return n;
}
