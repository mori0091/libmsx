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
 * \file vmem_rw.c
 */

#include "vmem_rw.h"

#include "bios.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "./vdp_internal.h"

static vmemptr_t vmem = (vmemptr_t)(-1);

bool vmem_is_opened(void) {
  return vmem <= 128UL * 1024;
}

void vmem_open(vmemptr_t loc) {
  assert(!vmem_is_opened());
  vmem_set_write_address(vmem = loc);
}

void vmem_close(void) {
  assert(vmem_is_opened());
  vmem = (vmemptr_t)(-1);
}

vmemptr_t vmem_ptr(void) {
  assert(vmem_is_opened());
  return vmem;
}

void vmem_write_chunk(const uint8_t * src, uint16_t len);

void vmem_skip(size_t len) {
  assert(vmem_is_opened());
  vmem += len;
  vmem_set_write_address(vmem);
}

#include "vdp.h"

#if (__SDCCCALL == 1)

#pragma disable_warning 218

// \note Supports only within a 16KiB aligned area of VRAM.
static void vmem_dup__16K(uint16_t src, uint16_t dst, size_t len) __naked {
  // assert(src < 0x4000 && dst < 0x4000 && 0 < len && dst + len <= 0x4000);
  (void)src;                    // HL
  (void)dst;                    // DE
  (void)len;                    // (SP+2)
  __asm__("        ld  iy, #0");
  __asm__("        add iy, sp");
  __asm__("        ld  b, +2 (iy)");
  __asm__("        ld  a, +3 (iy)");
  __asm__("        set 6, d");  // dst |= 0x4000
  __asm__("        ld  c, #_vdp_port1");
  __asm__("        inc b");
  __asm__("        dec b");
  __asm__("        jr  z, 00003$");
  __asm__("00002$: ; outer-loop");
  __asm__("        push af");
  __asm__("00001$: ; inner-loop");
  __asm__("        di");
  __asm__("        out (c), l");
  __asm__("        out (c), h");
  __asm__("        ei");        // 4
  __asm__("        inc hl");    // 6
  __asm__("        dec c");     // 4
  __asm__("        in  a, (c)");
  __asm__("        inc c");
  __asm__("        di");
  __asm__("        out (c), e");
  __asm__("        out (c), d");
  __asm__("        ei");        // 4
  __asm__("        inc de");    // 6
  __asm__("        dec c");     // 4
  __asm__("        out (c), a");
  __asm__("        inc c");
  __asm__("        djnz 00001$");
  __asm__("        pop af");
  __asm__("00003$: or  a");
  __asm__("        ret z");
  __asm__("        dec a");
  __asm__("        jr  00002$");
}

#else

// \note Supports only within a 16KiB aligned area of VRAM.
static void vmem_dup__16K(uint16_t src, uint16_t dst, size_t len) {
  assert(src < 0x4000 && dst < 0x4000 && dst + len <= 0x4000);
  dst |= 0x4000;
  while (len--) {
    {
      __asm__("di");
      vdp_port1 = src & 255;
      vdp_port1 = (src >> 8);
      __asm__("ei");
      src++;
      __asm__("nop");           // needs for MSX1
    }
    const uint8_t x = vdp_port0;
    {
      __asm__("di");
      vdp_port1 = dst & 255;
      vdp_port1 = (dst >> 8);
      __asm__("ei");
      dst++;
    }
    vdp_port0 = x;
  }
}

#endif

// \note Supports only lower 64KiB area of VRAM
static void vmem_dup__64K_1(uint16_t offset, size_t len) {
  assert(vmem + len <= 0x10000UL);
  uint16_t dst = (uint16_t)(vmem & 0xffff);
  while (len--) {
    {
      const uint16_t src = dst - offset;
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(src >> 14));
      vdp_port1 = src & 255;
      vdp_port1 = (src >> 8) & 0x3f;
      __asm__("ei");
    }
    const uint8_t x = vdp_port0;
    {
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(dst >> 14));
      vdp_port1 = dst & 255;
      vdp_port1 = ((dst >> 8) & 0x3f) | 0x40;
      __asm__("ei");
      dst++;
    }
    vdp_port0 = x;
  }
}

// \note Supports only upper 64KiB area of VRAM
static void vmem_dup__64K_2(uint16_t offset, size_t len) {
  assert(vmem - offset >= 0x10000UL);
  uint16_t dst = (uint16_t)(vmem & 0xffff);
  while (len--) {
    {
      const uint16_t src = dst - offset;
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(src >> 14) | 4);
      vdp_port1 = src & 255;
      vdp_port1 = (src >> 8) & 0x3f;
      __asm__("ei");
    }
    const uint8_t x = vdp_port0;
    {
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(dst >> 14) | 4);
      vdp_port1 = dst & 255;
      vdp_port1 = ((dst >> 8) & 0x3f) | 0x40;
      __asm__("ei");
      dst++;
    }
    vdp_port0 = x;
  }
}

// \note Supports all 128KiB area of VRAM. (slow)
static void vmem_dup__128K(uint16_t offset, size_t len) {
  assert(vmem + len <= 0x20000UL);
  vmemptr_t dst = vmem;
  while (len--) {
    {
      const vmemptr_t src = dst - offset;
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(src >> 14));
      vdp_port1 = src & 255;
      vdp_port1 = (src >> 8) & 0x3f;
      __asm__("ei");
    }
    const uint8_t x = vdp_port0;
    {
      __asm__("di");
      VDP_SET_CONTROL_REGISTER(14, (uint8_t)(dst >> 14));
      vdp_port1 = dst & 255;
      vdp_port1 = ((dst >> 8) & 0x3f) | 0x40;
      __asm__("ei");
      dst++;
    }
    vdp_port0 = x;
  }
}

void vmem_dup(uint16_t offset, size_t len) {
  assert(vmem_is_opened());
  {
    uint16_t dst = (uint16_t)vmem & 0x3fff;
    if (offset < 0x4000 && offset <= dst) {
      uint16_t src = dst - offset;
      uint16_t n = 0x4000 - dst;
      if (len <= n) {
        vmem += len;
        vmem_dup__16K(src, dst, len);
        return;
      }
      vmem += n;
      len -= n;
      vmem_dup__16K(src, dst, n);
    }
  }
  if (vmem + len <= 0x10000UL) {
    vmem_dup__64K_1(offset, len);
  }
  else if (0x10000UL <= vmem - offset) {
    vmem_dup__64K_2(offset, len);
  }
  else {
    vmem_dup__128K(offset, len);
  }
  vmem += len;
}
