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
 * \file NDP_load_bgm.c
 */

#include <NDP.h>
#include "./NDP__internal.h"
#include "memfile.h"

#include <bios.h>

#include <assert.h>

bool NDP_load_bgm(MemFile * ndp, uint8_t * buf, size_t buf_size) {
  uint32_t len = mfsize(ndp);
  if (buf && (len <= buf_size)) {
    assert((uint8_t *)PAGE_ADDR(3) <= buf);
    mfseek(ndp, 0, MEM_SEEK_SET);
    mfread(ndp, buf, len);
    NDP__set_song_ptr(0, buf);
    return true;
  }
  return false;
}
