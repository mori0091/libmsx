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
 * \file NDS__verify.c
 */

#include <memfile.h>

int NDS__verify(MemFile * mf) {
  mfseek(mf, 0, MEM_SEEK_SET);
  return (int)mfread_u8(mf);
}
