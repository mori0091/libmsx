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
 * \file zx0_decompress.c
 */

// #include "zx0_decompress.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static const uint8_t * src_ptr;
static uint8_t * dst_ptr;
static uint16_t last_offset;
static uint8_t bit_mask;
static uint8_t bit_value;

#if defined(__SDCC)

static void copy_from(const uint8_t * src, size_t len) {
  memcpy(dst_ptr, src, len);
  dst_ptr += len;
}

#else

static void copy_from(const uint8_t * src, size_t len) {
  // ** cannot use memcpy()/memmove() in this case. **
  // - copying by memcpy() of overlapping area is undefined behavior.
  // - memmove() does not causes side effect (repitition), expected in LZ77/LZSS
  //   algorithm.
  // memcpy(dst_ptr, src, len);
  // dst_ptr += len;
  while (len--) {
    *dst_ptr++ = *src++;
  }
}

#endif

static void duplicate_bytes(size_t len) {
  copy_from(dst_ptr - last_offset, len);
}

static void transfer_bytes(size_t len) {
  copy_from(src_ptr, len);
  src_ptr += len;
}

static uint8_t read_byte(void) {
  return *src_ptr++;
}

static bool read_bit(void) {
  bit_mask >>= 1;
  if (!bit_mask) {
    bit_mask = 0x80;
    bit_value = read_byte();
  }
  return !!(bit_value & bit_mask);
}

static uint16_t read_interlaced_elias_gamma(void) {
  uint16_t value = 1;
  while (!read_bit()) {
    value = (value << 1) | read_bit();
  }
  return value;
}

static uint16_t read_interlaced_elias_gamma_with_backtrack(uint8_t last_byte) {
  if (last_byte & 1) {
    return 2;
  }
  uint16_t value = 1;
  do {
    value = (value << 1) | read_bit();
  } while (!read_bit());
  return value + 1;
}

static uint16_t read_interlaced_elias_gamma_inverted(void) {
  uint16_t value = 1;
  while (!read_bit()) {
    value = (value << 1) | !read_bit();
  }
  return value;
}

size_t zx0_decompress(const uint8_t * src, uint8_t * dst) {
  src_ptr = src;
  dst_ptr = dst;
  last_offset = 1;
  bit_mask = 0;

  for (;;) {
    do {
      // COPY_LITERALS:
      transfer_bytes(read_interlaced_elias_gamma());
      if (read_bit()) {
        break;
      }
      // COPY_FROM_LAST_OFFSET:
      duplicate_bytes(read_interlaced_elias_gamma());
    } while (!read_bit());

    do {
      // COPY_FROM_NEW_OFFSET:
      uint16_t offset_hi = read_interlaced_elias_gamma_inverted();
      if (offset_hi == 256) {
        return dst_ptr - dst;
      }
      uint8_t offset_lo = read_byte();
      last_offset = offset_hi * 128 - (offset_lo >> 1);
      duplicate_bytes(read_interlaced_elias_gamma_with_backtrack(offset_lo));
    } while (read_bit());
  }
}
