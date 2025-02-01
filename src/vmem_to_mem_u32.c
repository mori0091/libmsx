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
 * \file vmem_to_mem_u32.c
 */

#include "xmem.h"

#include "bios.h"

#include <assert.h>

// wrapper of `vmem_to_mem(size_t)`
void vmem_to_mem_u32(uint32_t len) {
  assert(len <= UINT16_MAX);
  vmem_to_mem((size_t)len);
}
