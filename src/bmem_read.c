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
 * \file bmem_read.c
 */

#include "bmem.h"

#include "xmem.h"

void bmem_read(bmemptr_t loc, void * dst, uint16_t len) {
  mem_open((uint8_t *)dst);
  bmem_open(loc);
  bmem_to_mem(len);
  bmem_close();
  mem_close();
}
