// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2026 Daishi Mori (mori0091)
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

bool NDP_load_bgm(NDPFile * ndp, uint8_t * buf, size_t buf_size) {
  if (NDP__load_data(&ndp->mf, buf, buf_size)) {
    NDP__set_song_ptr(0, buf);
    return true;
  }
  return false;
}
