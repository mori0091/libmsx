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
 * \file ZX0_decompress_bmem.c
 */

#include "ZX0_decompress.h"
#include "xmem.h"

#include "./ZX0__decode.h"

size_t ZX0_decompress_bmem(bmemptr_t src, uint8_t * dst) {
  ZX0__fnptr_duplicate = mem_dup;
  ZX0__fnptr_transfer  = bmem_to_mem;
  ZX0__fnptr_read_byte = bmem_read_byte;
  bmem_open(src);
  mem_open(dst);
  ZX0__decode();
  size_t s = mem_ptr() - dst;
  mem_close();
  bmem_close();
  return s;
}
