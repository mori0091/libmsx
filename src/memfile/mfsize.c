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
 * \file mfsize.c
 */

#include <memfile.h>

uint32_t mfsize(struct MemFile * mf) {
  return mfpos(&mf->end) - mfpos(&mf->base);
}
