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
 * \file ZX0__decode.c
 *
 * Decompresser of ZX0 compressed file format version 2.
 *
 * The ZX0 data compression format and algorithm was designed by Einar Saukas.
 *
 * \sa https://github.com/einar-saukas/ZX0
 *
 * This C implementation of the ZX0 decompressor has been reimplemented for use
 * with libmsx by Daishi Mori (mori0091). It provides only the core part of the
 * ZX0 decompressor, which is independent of input/output memory and storage
 * type.
 */

#include "ZX0__decode.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint8_t bit_mask;
static uint8_t bit_value;

void (* ZX0__fnptr_duplicate)(uint16_t offset, size_t len);
void (* ZX0__fnptr_transfer)(size_t len);
uint8_t (* ZX0__fnptr_read_byte)(void);

static inline void duplicate_bytes(uint16_t offset, size_t len) {
  ZX0__fnptr_duplicate(offset, len);
}

static inline void transfer_bytes(size_t len) {
  ZX0__fnptr_transfer(len);
}

static inline uint8_t read_byte(void) {
  return ZX0__fnptr_read_byte();
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

static uint8_t read_interlaced_elias_gamma_inverted(void) {
  uint8_t value = 1;
  while (!read_bit()) {
    value = (value << 1) | !read_bit();
  }
  return value;
}

static uint16_t read_interlaced_elias_gamma_with_backtrack_0(void) {
  uint16_t value = 1;
  do {
    value = (value << 1) | read_bit();
  } while (!read_bit());
  return value + 1;
}

void ZX0__decode(void) {
  uint16_t offset = 1;
  bit_mask = 0;
  for (;;) {
    do {
      // COPY LITERALS
      transfer_bytes(read_interlaced_elias_gamma());
      if (read_bit()) {
        break;
      }
      // COPY FROM THE LAST OFFSET
      duplicate_bytes(offset, read_interlaced_elias_gamma());
    } while (!read_bit());

    do {
      // COPY FROM NEW OFFSET
      const uint8_t offset_hi = read_interlaced_elias_gamma_inverted();
      if (!offset_hi) {
        return;
      }
      const uint8_t offset_lo = read_byte();
      offset = (uint16_t)offset_hi * 128 - (offset_lo >> 1);
      const uint16_t len = (offset_lo & 1) ? 2 : read_interlaced_elias_gamma_with_backtrack_0();
      duplicate_bytes(offset, len);
    } while (read_bit());
  }
}
