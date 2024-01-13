// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file mfread.c
 */

#include <memfile.h>

size_t mfread(MemFile * mf, void * ptr, size_t size) {
  return mf->methods->read(mf, ptr, size);
}
