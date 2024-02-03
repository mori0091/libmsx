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
 * \file mfopen_mem.c
 */

#include <memfile.h>

#include <stdint.h>
#include <string.h>

static uint8_t read1(MemFile * mf) {
  uint8_t ret = *(mf->curr.ptr);
  if (mf->curr.ptr < mf->end.ptr) {
    mf->curr.ptr++;
  }
  return ret;
}

static size_t read(MemFile * mf, void * ptr, size_t size) {
  const size_t rem = mf->end.ptr - mf->curr.ptr;
  if (rem < size) {
    size = rem;
  }
  memcpy(ptr, mf->curr.ptr, size);
  mf->curr.ptr += size;
  return size;
}

static void seek(mempos_t * dst, const mempos_t * src, long offset) {
  dst->ptr = src->ptr + offset;
  dst->segment = 0;
  dst->slot = 0;
}

static const struct MemFileMethods methods = {
.read1 = read1,
.read  = read,
.seek  = seek,
};

void mfopen_mem(MemFile * mf, uint8_t * p, size_t size) {
  memset(mf, 0, sizeof(MemFile));
  if (!p) return;
  const uint8_t * const end = p + size;
  if (p <= end && end < (uint8_t *)0xffff) {
    mf->base.ptr = p;
    mf->curr.ptr = p;
    mf->end.ptr  = p + size;
    mf->methods = &methods;
  }
}
