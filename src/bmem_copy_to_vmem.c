// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
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

// Please execute this on Z80 mode, if neccessary, to avoid excessive waiting
// time inserted by S1990 though not recommended.
// (i.e. Z80 mode is faster than R800 mode in this case!)
// See also https://www.msx.org/wiki/VRAM_access_speed
// and issue #83 (https://github.com/mori0091/libmsx/issues/83)
void bmem_copy_to_vmem(bmemptr_t src, vmemptr_t dst, uint32_t len) {
  vmem_open(dst);
  bmem_open(src);
  bmem_to_vmem_u32(len);
  bmem_close();
  vmem_close();
}
