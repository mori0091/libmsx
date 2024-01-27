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
 * \file mfsetpos2.c
 */

#include <memfile.h>

// void mfsetpos2(MemFile * mf, const mempos_t * base, long offset) {
//   mfsetpos(mf, base);
//   mfseek(mf, offset, MEM_SEEK_CUR);
// }
void mfsetpos2(MemFile * mf, const mempos_t * base, long offset) {
  mf->methods->seek(&mf->curr, base, offset);
}
