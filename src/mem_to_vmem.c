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
 * \file mem_to_vmem.c
 */

#include "xmem.h"

#include "bios.h"

#include <assert.h>

void mem_to_vmem(size_t len) {
  assert(mem_is_opened() && vmem_is_opened());
  // vmem_set_write_address(vmem_ptr());
  vmem_write_chunk(mem_ptr(), len);
}
