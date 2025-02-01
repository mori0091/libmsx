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
 * \file mfgetpos.c
 */

#include <memfile.h>

#include <string.h>

void mfgetpos(MemFile * mf, mempos_t * pos) {
  memcpy(pos, &mf->curr, sizeof(mempos_t));
}
