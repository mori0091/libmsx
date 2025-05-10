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

size_t NDP_read_metadata(MemFile * ndp, uint8_t * buf, size_t buf_size) {
  if (!NDP_has_metadata(ndp) || !buf || buf_size < 5) return 0;
  mfseek(ndp, 0x0e, MEM_SEEK_SET);
  uint8_t * p = buf;
  for (uint8_t n = 5; n; n--) {
    // Read Shift-JIS string which ends with '0xff'.
    for (uint8_t c; n < buf_size && (c = mfread_u8(ndp)) != 0xff; ) {
      if (0x80 <= c) {
        uint8_t c2 = mfread_u8(ndp);
        if (buf_size <= n+1) break;
        *p++ = c;
        *p++ = c2;
        buf_size -= 2;
      }
      else {
        *p++ = c;
        buf_size--;
      }
    }
    *p++ = '\0';
    buf_size--;
  }
  return p - buf;
}
