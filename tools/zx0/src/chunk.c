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
 * \file chunk.c
 */

#include "chunk.h"

#include "zx0.h"

// -----------------------------------------------------------------------

/**
 * Construct a chunk that represents LITERAL.
 * \param index  The last position of input in bytes.
 * \param length Length in bytes.
 * \return Intermediate representation of LITERAL.
 */
Chunk Literal(size_t index, uint16_t length) {
  return (Chunk) {
    .bits_cost = (1 + bit_length_of_elias_gamma(length) + 8 * length),
    .index = index,
    .offset = 0,
  };
}
/**
 * Construct a chunk that represents COPY_FROM_LAST_OFFSET.
 * \param index  The last position of input in bytes.
 * \param offset The offset of the matched portion (should be same as the last offset).
 * \param length Length in bytes.
 * \return Intermediate representation of COPY_FROM_LAST_OFFSET.
 */
Chunk MatchFromLastOffset(size_t index, uint16_t offset, uint16_t length) {
  return (Chunk) {
    .bits_cost = 1 + bit_length_of_elias_gamma(length),
    .index = index,
    .offset = offset,
  };
}
/**
 * Construct a chunk that represents COPY_FROM_NEW_OFFSET.
 * \param index  The last position of input in bytes.
 * \param offset The offset of the matched portion (should be differ from the last offset).
 * \param length Length in bytes.
 * \return Intermediate representation of COPY_FROM_NEW_OFFSET.
 */
Chunk MatchFromNewOffset(size_t index, uint16_t offset, uint16_t length) {
  const int offset_hi = ((int)offset - 1) / 128 + 1;
  return (Chunk) {
    .bits_cost = (8 + bit_length_of_elias_gamma(offset_hi) + bit_length_of_elias_gamma(length - 1)),
    .index = index,
    .offset = offset,
  };
}

/**
 * Tests if a chunk represents LITERAL or not.
 * \param c A chunk.
 * \return `true` if `c` represents LITERAL.
 */
bool is_Literal(Chunk c) {
  return !c.offset;
}
/**
 * Tests if a chunk represents COPY_FROM_LAST_OFFSET or not.
 * \param c A chunk.
 * \param last_offset The last offset value.
 * \return `true` if `c` represents COPY_FROM_LAST_OFFSET.
 */
bool is_MatchFromLastOffset(Chunk c, uint16_t last_offset) {
  return c.offset && (c.offset == last_offset);
}
/**
 * Tests if a chunk represents COPY_FROM_NEW_OFFSET or not.
 * \param c A chunk.
 * \param last_offset The last offset value.
 * \return `true` if `c` represents COPY_FROM_NEW_OFFSET.
 */
bool is_MatchFromNewOffset(Chunk c, uint16_t last_offset) {
  return c.offset && (c.offset != last_offset);
}
