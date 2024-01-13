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
 * \file mfsetpos.c
 */

#include <memfile.h>

#include <string.h>

void mfsetpos(MemFile * mf, const mempos_t * pos) {
  memcpy(&mf->curr, pos, sizeof(mempos_t));
}
