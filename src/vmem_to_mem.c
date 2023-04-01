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
 * \file vmem_to_mem.c
 */

#include "xmem.h"

#include "bios.h"

#include <assert.h>

void vmem_to_mem(size_t len) {
  assert(mem_is_opened() && vmem_is_opened());
  vmem_set_read_address(vmem_ptr());
  vmem_read_chunk(mem_ptr(), len);
  vmem_set_write_address(vmem_ptr());
}
