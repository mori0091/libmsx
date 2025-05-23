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
 * \file mfeof.c
 */

#include <memfile.h>
#include <stdint.h>

bool mfeof(MemFile * mf) {
  if (mf->curr.segment < mf->end.segment) return false;
  if (mf->curr.segment > mf->end.segment) return true;
  return mf->curr.ptr >= mf->end.ptr;
}
