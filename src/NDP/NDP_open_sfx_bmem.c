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
 * \file NDP_open_sfx_bmem.c
 */

#include <NDP.h>

extern int NDS__verify(MemFile * mf);

int NDP_open_sfx_bmem(NDSFile * nds, bmemptr_t loc, uint32_t size) {
  mfopen_bmem(&nds->mf, loc, size);
  return NDS__verify(&nds->mf);
}
