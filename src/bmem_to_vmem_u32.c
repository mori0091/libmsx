// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem_to_vmem_u32.c
 */

#include "xmem.h"

#include "bios.h"

#include <assert.h>

void bmem_to_vmem_u32(uint32_t len) {
  assert(bmem_is_opened() && vmem_is_opened());
  // vmem_set_write_address(vmem_ptr());
  bmem_ensure();
  size_t n = bmem_avail();
  while (n <= len) {
    vmem_write_chunk(bmem_ptr(), n);
    vmem_skip(n);
    len -= n;
    n = PAGE_SIZE;
    bmem_next_segment();
  }
  if (len) {
    vmem_write_chunk(bmem_ptr(), len);
    vmem_skip(len);
    bmem_skip(len);
  }
}
