// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_read_address.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_set_read_address(vmemptr_t loc) {
  __asm__("di");
  VDP_SET_VMEM_READ_POINTER(loc);
  __asm__("ei");
}
