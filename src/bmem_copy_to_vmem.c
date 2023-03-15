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
 * \file bmem_copy_to_vmem.c
 */

#include "bmem.h"

#include "bios.h"
#include "xmem.h"

void bmem_copy_to_vmem(bmemptr_t src, vmemptr_t dst, uint32_t len) {
  vmem_open(dst);
  bmem_open(src);
  // Force Z80 mode to avoid excessive waiting time inserted by S1990.
  // (i.e. Z80 mode is faster than R800 mode in this case!)
  // See also https://www.msx.org/wiki/VRAM_access_speed
  const uint8_t cpu = msx_get_cpu_mode();
  msx_set_cpu_mode(0x00);
  bmem_to_vmem_u32(len);
  msx_set_cpu_mode(cpu);
  bmem_close();
  vmem_close();
}
