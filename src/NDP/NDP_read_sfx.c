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
 * \file NDP_read_sfx.c
 */

#include <NDP.h>
#include "./NDP__internal.h"

size_t NDP_read_sfx(uint8_t index, NDSFile * nds, uint8_t * buf, size_t buf_size) {
  MemFile * mf = &nds->mf;
  mfseek(mf, 0, MEM_SEEK_SET);
  const uint8_t num = mfread_u8(mf);
  if (num <= index || !buf) return 0;
  mfseek(mf, 2 * index, MEM_SEEK_CUR);
  const uint16_t beg = mfread_u16(mf);
  const uint16_t end = (index+1 == num) ? (uint16_t)mfsize(mf) : mfread_u16(mf);
  if (end <= beg) return 0;
  const size_t sz = end - beg;
  if (buf_size < sz) return 0;
  mfseek(mf, beg, MEM_SEEK_SET);
  return mfread(mf, buf, sz);
}
