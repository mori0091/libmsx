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
 * \file la0_open_mem.c
 */

#include "la0__internal.h"

int la0_open_mem(MemFile * mf, const uint8_t * loc, size_t size) {
  if (!mf || !loc || !size) return 0;
  mfopen_mem(mf, (uint8_t *)loc, size);
  return la0__count_songs(mf);
}
