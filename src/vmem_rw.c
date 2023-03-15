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
 * \file vmem_rw.c
 */

#include "vmem_rw.h"

#include "bios.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vmemptr_t vmem = (vmemptr_t)(-1);

bool vmem_is_opened(void) {
  return vmem < 128UL * 1024;
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

void vmem_copy_b(vmemptr_t src, vmemptr_t dst, uint32_t len, uint8_t * buf, size_t buf_len) {
  assert(buf && buf_len && buf_len <= 0xffff - (uintptr_t)buf);
  while (len) {
    if (len < buf_len) {
      buf_len = len;
    }
    vmem_read(src, buf, buf_len);
    vmem_write(dst, buf, buf_len);
    src += buf_len;
    dst += buf_len;
    len -= buf_len;
  }
}

static uint8_t tmp_buf[1024];

void vmem_copy(vmemptr_t src, vmemptr_t dst, uint32_t len) {
  vmem_copy_b(src, dst, len, tmp_buf, sizeof(tmp_buf));
}

void vmem_copy_opt2(vmemptr_t src, vmemptr_t dst, uint32_t len) {
  uint8_t slot_p2 = msx_get_slot(PAGE_ADDR(2));
  msx_ENASLT(msx_get_slot(PAGE_ADDR(3)), PAGE_ADDR(2));
  vmem_copy_b(src, dst, len, PAGE_ADDR(2), PAGE_SIZE);
  msx_ENASLT(slot_p2, PAGE_ADDR(2));
  __asm__("ei");
}

#include "vdp.h"

// \note Supports only within a 16KiB aligned area of VRAM.
void vmem_dup__16K(uint16_t src, uint16_t dst, size_t len) {
  assert(src < 0x4000 && dst < 0x4000 && dst + len <= 0x4000);
  dst |= 0x4000;
  while (len--) {
    {
      __asm__("di");
      vdp_port1 = src & 255;
      vdp_port1 = (src >> 8);
      __asm__("ei");
      src++;
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

// \note Supports only lower 64KiB area of VRAM
void vmem_dup__64K_1(uint16_t offset, size_t len) {
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
void vmem_dup__64K_2(uint16_t offset, size_t len) {
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
void vmem_dup__128K(uint16_t offset, size_t len) {
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
  uint16_t dst = (uint16_t)vmem & 0x3fff;
  if (offset < 0x4000 && offset <= dst) {
    uint16_t src = dst - offset;
    uint16_t n = 0x4000 - dst;
    if (len <= n) {
      vmem_dup__16K(src, dst, len);
      vmem += len;
      return;
    }
    vmem_dup__16K(src, dst, n);
    vmem += n;
    len -= n;
    // dst = 0;
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
