// -*- coding: utf-8-unix -*-
/**
 * \file vmem_read.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_read(vmemptr_t src, void* dst, uint16_t len) {
  __critical {
    vmem_set_read_address(src);
    for (uint8_t* p = dst; len--; ) {
      *p++ = vdp_port0;
    }
  }
}
