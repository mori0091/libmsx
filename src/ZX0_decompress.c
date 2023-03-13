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
 * \file ZX0_decompress.c
 */

#include "ZX0_decompress.h"
#include "xmem.h"

#include "./ZX0__decode.h"

static const uint8_t * src_ptr;

static void default_transfer(size_t len) {
  mem_write_chunk(src_ptr, len);
  src_ptr += len;
}

static uint8_t default_read_byte(void) {
  return *src_ptr++;
}

size_t ZX0_decompress(const uint8_t * src, uint8_t * dst) {
  ZX0__fnptr_duplicate = mem_dup;
  ZX0__fnptr_transfer  = default_transfer;
  ZX0__fnptr_read_byte = default_read_byte;
  src_ptr = src;
  mem_open(dst);
  ZX0__decode();
  size_t s = mem_ptr() - dst;
  mem_close();
  return s;
}
