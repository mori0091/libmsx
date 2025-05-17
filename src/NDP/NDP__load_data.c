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
 * \file NDP__load_data.c
 */

#include <NDP.h>
#include "./NDP__internal.h"

#include <bios.h>

#include <assert.h>

bool NDP__load_data(MemFile * mf, void * buf, size_t buf_size) {
  assert(PAGE_ADDR(3) <= buf);
  const uint32_t len = mfsize(mf);
  if (buf && (len <= buf_size)) {
    mfseek(mf, 0, MEM_SEEK_SET);
    mfread(mf, buf, len);
    return true;
  }
  return false;
}
