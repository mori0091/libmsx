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
 * \file mfseek.c
 */

#include <memfile.h>

#include <assert.h>

void mfseek(MemFile * mf, long offset, int whence) {
  assert(MEM_SEEK_SET <= whence && whence <= MEM_SEEK_END);
  mf->methods->seek(&mf->curr, (const mempos_t *)mf + whence, offset);
}
