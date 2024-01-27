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
 * \file ZX0_decompress_bmem_to_vmem.c
 */

#include "ZX0_decompress.h"
#include "xmem.h"

#include "./ZX0__decode.h"

uint32_t ZX0_decompress_bmem_to_vmem(bmemptr_t src, vmemptr_t dst) {
  ZX0__fnptr_duplicate = vmem_dup;
  ZX0__fnptr_transfer  = bmem_to_vmem;
  ZX0__fnptr_read_byte = bmem_read_byte;
  bmem_open(src);
  vmem_open(dst);
  ZX0__decode();
  uint32_t s = vmem_ptr() - dst;
  vmem_close();
  bmem_close();
  return s;
}
