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

void vmem_dup_b(uint16_t offset, size_t len, uint8_t * buf, size_t buf_len) {
  assert(vmem_is_opened());

  if (buf_len < offset) {
    vmem_copy_b(vmem - offset, vmem, len, buf, buf_len);
    vmem += len;
    return;
  }

  vmem_read(vmem - offset, buf, offset);
  size_t n = offset;
  while (offset <= buf_len - n) {
    memcpy(buf + n, buf, offset);
    n += offset;
  }
  vmem_set_write_address(vmem);
  vmem += len;
  while (len) {
    if (len < n) {
      n = len;
    }
    vmem_write_chunk(buf, n);
    len -= n;
  }
}

void vmem_dup(uint16_t offset, size_t len) {
  assert(vmem_is_opened());
  vmem_dup_b(offset, len, tmp_buf, sizeof(tmp_buf));
}

void vmem_dup_opt2(uint16_t offset, size_t len) {
  assert(vmem_is_opened());
  uint8_t slot_p2 = msx_get_slot(PAGE_ADDR(2));
  msx_ENASLT(msx_get_slot(PAGE_ADDR(3)), PAGE_ADDR(2));
  vmem_dup_b(offset, len, PAGE_ADDR(2), PAGE_SIZE);
  msx_ENASLT(slot_p2, PAGE_ADDR(2));
  __asm__("ei");
}
