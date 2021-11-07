// -*- coding: utf-8-unix -*-
/**
 * \file vmem_set_read_address.c
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

void vmem_set_read_address(vmemptr_t loc) {
  VDP_SET_CTRL(14, (uint8_t)(((loc) >> 14) & 7));
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F);
}
