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
 * \file mfslice.c
 */

#include <memfile.h>

#include <string.h>

void mfslice(MemFile * dst, const MemFile * src, long size) {
  memcpy(&dst->base, &src->curr, sizeof(mempos_t));
  memcpy(&dst->curr, &src->curr, sizeof(mempos_t));
  src->methods->seek(&dst->end, &src->curr, size);
  dst->methods = src->methods;
}
