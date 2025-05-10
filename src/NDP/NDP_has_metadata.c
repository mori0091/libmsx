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
 * \file NDP_has_metadata.c
 */

#include <NDP.h>

bool NDP_has_metadata(MemFile * ndp) {
  mfseek(ndp, 0x0b, MEM_SEEK_SET);
  return (mfread_u8(ndp) & 2);
}
