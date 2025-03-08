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
 * \file mfopen_bmem.c
 */

#include <memfile.h>

#include "../memmap.h"

#include <stdint.h>
#include <string.h>

#define bmem_offset_of(loc) (uint16_t)((loc) & (PAGE_SIZE - 1))

static void mempos_from_bmemptr(mempos_t * pos, bmemptr_t p) {
  pos->ptr = (uint8_t *)PAGE_ADDR(2) + bmem_offset_of(p);
  pos->segment = bmem_bank_of(p);
  pos->slot = CARTRIDGE_SLOT;
}

static uint8_t read1(MemFile * mf) {
  struct MemMap saved_state;
  memmap_save(&saved_state);
  memmap_expose_cartridge();

  const uint8_t bank = bmem_get_bank();
  bmem_set_bank(mf->curr.segment);
  uint8_t ret = *mf->curr.ptr++;
  if ((uint8_t *)PAGE_ADDR(3) == mf->curr.ptr) {
    mf->curr.ptr = (uint8_t *)PAGE_ADDR(2);
    mf->curr.segment++;
  }
  bmem_set_bank(bank);

  memmap_restore(&saved_state);

  return ret;
}

static size_t read(MemFile * mf, void * ptr, size_t size) {
  struct MemMap saved_state;
  memmap_save(&saved_state);
  memmap_expose_cartridge();

  const uint8_t bank = bmem_get_bank();
  uint8_t * dst = ptr;
  size_t len = size;
  size_t n = (uint8_t *)PAGE_ADDR(3) - mf->curr.ptr;
  if (n <= len) {
    bmem_set_bank(mf->curr.segment++);
    memcpy(dst, mf->curr.ptr, n);
    dst += n;
    len -= n;
    mf->curr.ptr = (uint8_t *)PAGE_ADDR(2);
  }
  while (PAGE_SIZE <= len) {
    bmem_set_bank(mf->curr.segment++);
    memcpy(dst, (uint8_t *)PAGE_ADDR(2), PAGE_SIZE);
    dst += PAGE_SIZE;
    len -= PAGE_SIZE;
  }
  if (len) {
    bmem_set_bank(mf->curr.segment);
    memcpy(dst, mf->curr.ptr, len);
    mf->curr.ptr += len;
  }
  bmem_set_bank(bank);

  memmap_restore(&saved_state);

  return size;
}

static void seek(mempos_t * dst, const mempos_t * src, long offset) {
  bmemptr_t p
    = (bmemptr_t)src->segment * PAGE_SIZE
    + bmem_offset_of((uintptr_t)src->ptr)
    + offset;
  dst->ptr = (uint8_t *)PAGE_ADDR(2) + bmem_offset_of(p);
  dst->segment = bmem_bank_of(p);
  dst->slot = CARTRIDGE_SLOT;
}

static const struct MemFileMethods methods = {
.read1 = read1,
.read  = read,
.seek  = seek,
};

void mfopen_bmem(MemFile * mf, bmemptr_t p, uint32_t size) {
  memset(mf, 0, sizeof(MemFile));
  if (p) {
    mempos_from_bmemptr(&mf->base, p);
    mempos_from_bmemptr(&mf->curr, p);
    mempos_from_bmemptr(&mf->end , p + size);
    mf->methods = &methods;
  }
}
