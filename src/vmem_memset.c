// -*- coding: utf-8-unix -*-
/**
 * \file vmem_memset.c
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

void vmem_memset(vmemptr_t dst, uint8_t val, uint16_t len) {
  __critical {
    vmem_set_write_address(dst);
    for (; len--; ) {
      vdp_port0 = val;
    }
  }
}
