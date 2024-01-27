// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
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
  // Force Z80 mode to avoid excessive waiting time inserted by S1990.
  // (i.e. Z80 mode is faster than R800 mode in this case!)
  // See also https://www.msx.org/wiki/VRAM_access_speed
  const uint8_t cpu = msx_get_cpu_mode();
  msx_set_cpu_mode(0x00);
  vmem_write_chunk((const uint8_t *)src, len);
  msx_set_cpu_mode(cpu);
}
