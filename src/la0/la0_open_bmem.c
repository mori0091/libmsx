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
 * \file la0_open_bmem.c
 */

#include "la0__internal.h"
#include "memfile.h"

int la0_open_bmem(MemFile * mf, bmemptr_t loc, uint32_t size) {
  if (!mf) return 0;
  mfopen_bmem(mf, loc, size);
  return la0__count_songs(mf);
}
