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
#include <stdint.h>

#if 0

void vmem_write_chunk(const uint8_t * p, uint16_t n);

void bmem_copy_to_vmem(bmemptr_t src, vmemptr_t dst, uint32_t len) {
  uint8_t bank = bmem_bank_of(src);
  const uint16_t offset = (uint16_t)(src & 0x3fff);
  const uint8_t * p = (const uint8_t *)0x8000 + offset;
  uint16_t n = (uint16_t)0x4000 - offset;

  const uint8_t old_bank = bmem_get_bank();
  vmem_set_write_address(dst);
  while (len) {
    if (len < n) {
      n = len;
    }
    len -= n;
    bmem_set_bank(bank);
    vmem_write_chunk(p, n);
    bank++;
    p = (const uint8_t *)0x8000;
    n = (uint16_t)0x4000;
  }
  bmem_set_bank(old_bank);
}

#else

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

#endif
