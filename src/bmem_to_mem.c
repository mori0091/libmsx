// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem_to_mem.c
 */

#include "xmem.h"

#include "bios.h"

#include <assert.h>

void bmem_to_mem(size_t len) {
  assert(bmem_is_opened() && mem_is_opened());
  bmem_ensure();
  size_t n = bmem_avail();
  while (n <= len) {
    mem_write_chunk(bmem_ptr(), n);
    len -= n;
    n = PAGE_SIZE;
    bmem_next_segment();
  }
  if (len) {
    mem_write_chunk(bmem_ptr(), len);
    bmem_skip(len);
  }
}
