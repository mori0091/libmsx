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
 * \file compress.c
 */

#include "zx0.h"

#include "array.h"
#include "file_io.h"
#include "info.h"
#include "options.h"

#include <assert.h>
#include <stdlib.h>

uint16_t last_offset;

bool backtrack;
uint8_t bit_mask;
uint8_t * bit_value_ptr;
uint8_t * dst_ptr;

/**
 * Write an octet to the ouput.
 * \param x An octet.
 */
void write_byte(uint8_t x) {
  assert(dst_ptr < encoded + sizeof(encoded));
  *dst_ptr++ = x;
}

/**
 * Write a sequence of octets to the ouput.
 * \param src Pointer to the head of the sequence.
 * \param len Length in bytes.
 */
void write_bytes(const uint8_t * src, size_t len) {
  while (len--) {
    write_byte(*src++);
  }
}

/**
 * Write a bit to the ouput.
 * \param x A bit.
 */
void write_bit(bool x) {
  if (backtrack) {
    if (x) {
      dst_ptr[-1] |= 1;
    }
    backtrack = false;
    return;
  }
  if (!bit_mask) {
    assert(dst_ptr < encoded + sizeof(encoded));
    bit_mask = 0x80;
    bit_value_ptr = dst_ptr;
    *dst_ptr++ = 0;
  }
  if (x) {
    *bit_value_ptr |= bit_mask;
  }
  bit_mask >>= 1;
}

/**
 * Encode an integer value to an interlaced elias-gamma code and write to
 * output.
 * \param x an integer value.
 */
void write_interlaced_elias_gamma(uint16_t x) {
  uint16_t mask = 2;
  while (mask <= x) {
     mask <<= 1;
  }
  mask >>= 1;
  while (mask >>= 1) {
    write_bit(false);
    write_bit(!!(x & mask));
  }
  write_bit(true);
}

/**
 * Encode an integer value to an inverted and interlaced elias-gamma code and
 * write to output.
 * \param x an integer value.
 */
void write_interlaced_elias_gamma_inverted(uint16_t x) {
  uint16_t mask = 2;
  while (mask <= x) {
     mask <<= 1;
  }
  mask >>= 1;
  while (mask >>= 1) {
    write_bit(false);
    write_bit(!(x & mask));
  }
  write_bit(true);
}

static void compress_begin(void) {
  dst_ptr = encoded;
  bit_mask = 0;
  backtrack = false;
  last_offset = 1;
}

static void compress_do(const uint8_t * beg, const uint8_t * end) {
  List(Chunk) rs = optimize(beg, end);
  size_t index = 0;
  // uint16_t last_offset = 1;
  const uint8_t * src = beg;
  DEBUG_INFO("---");
  while (rs) {
    const uint16_t len = rs->head.index - index;
    if (is_Literal(rs->head)) {
      assert(rs->head.bits_cost == 1 + bit_length_of_elias_gamma(len) + 8 * len);
      DEBUG_INFO("\n(%zu)", len);
      if (beg < src) {
        write_bit(false);
      }
      write_interlaced_elias_gamma(len);
      write_bytes(src, len);
      src += len;
    }
    else if (is_MatchFromLastOffset(rs->head, last_offset)) {
      assert(rs->head.bits_cost == 1 + bit_length_of_elias_gamma(len));
      DEBUG_INFO("[%zu]", len);
      write_bit(false);
      write_interlaced_elias_gamma(len);
      src += len;
    }
    else if (is_MatchFromNewOffset(rs->head, last_offset)) {
      assert(rs->head.bits_cost == 8 + bit_length_of_elias_gamma((rs->head.offset - 1) / 128 + 1) + bit_length_of_elias_gamma(len-1));
      DEBUG_INFO("[%d,%zu]", (int)rs->head.offset, len);
      write_bit(true);
      const uint8_t offset_hi = (rs->head.offset - 1) / 128 + 1;
      const uint8_t offset_lo = (127 - (rs->head.offset - 1) % 128) << 1;
      write_interlaced_elias_gamma_inverted((uint16_t)offset_hi);
      write_byte(offset_lo);
      backtrack = true;
      write_interlaced_elias_gamma(len - 1);
      src += len;
      last_offset = rs->head.offset;
    }
    else {
      ERROR("internal error: invalid chunk");
    }
    index = rs->head.index;
    rs = drop(rs);
  }
  DEBUG_INFO("\n---\n");

  run_gc();
}

static void compress_end(void) {
  DEBUG_INFO("EOM\n");
  write_bit(true);
  write_interlaced_elias_gamma_inverted((uint16_t)256);
}

/**
 * Compress.
 * \param src Pointer to the head of input.
 * \param len Length in bytes.
 * \return Length of output in bytes.
 */
size_t compress(const uint8_t * src, size_t len) {
  const uint8_t * beg = src;
  compress_begin();
  if (opt_align) {
    while (opt_align < len) {
      compress_do(beg, beg + opt_align);
      write_bit(false);         // Force the next block to start with a literal.
      beg += opt_align;
      len -= opt_align;
    }
  }
  compress_do(beg, beg + len);
  compress_end();
  return dst_ptr - encoded;
}
