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
 * \file vmem_write.c
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_write_chunk(const uint8_t * p, uint16_t n);

void vmem_write(vmemptr_t dst, void* src, uint16_t len) {
  vmem_set_write_address(dst);
  vmem_write_chunk((const uint8_t *)src, len);
}
