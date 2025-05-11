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
 * \file NDP_set_bgm.c
 */

#include <NDP.h>
#include "./NDP__internal.h"
#include "memfile.h"

#include <bios.h>

bool NDP_set_bgm(NDPFile * ndp) {
  if (ndp->mf.base.slot == 0 || ndp->mf.base.ptr + mfsize(&ndp->mf) <= (uint8_t *)PAGE_ADDR(3)) {
    uint8_t * ptr = ndp->mf.base.ptr;
    uint8_t bank = ndp->mf.base.segment;
    NDP__set_song_ptr(bank, ptr);
    return true;
  }
  return false;
}
