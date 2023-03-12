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
 * \file vmem_read.c
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_read_chunk(uint8_t * p, uint16_t n);

void vmem_read(vmemptr_t src, void* dst, uint16_t len) {
  vmem_set_read_address(src);
  vmem_read_chunk(dst, len);
}
