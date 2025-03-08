// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_write_address.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "interrupt.h"
#include "vdp_internal.h"

void vmem_set_write_address(vmemptr_t loc) {
  bool b = get_interrupt_state();
  __asm__("di");
  VDP_SET_VMEM_WRITE_POINTER(loc);
  if (b) { __asm__("ei"); }
}
