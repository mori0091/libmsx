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
 * \file NDP_load_sfx.c
 */

#include <NDP.h>

bool NDP_load_sfx(uint8_t index, NDSFile * nds, uint8_t * buf, size_t buf_size) {
  if (NDP_read_sfx(index, nds, buf, buf_size)) {
    NDP_set_sfx_ptr(buf);
    return true;
  }
  return false;
}
