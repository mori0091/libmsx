// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file chunk.h
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct chunk {
  int bits_cost;       // Number of bits of the corresponding output sequence.
  size_t index;        // Length of the input byte sequence from the beginning.
  uint16_t offset;     // `0` if literal block, `offset` value if matched block.
} Chunk;

/**
 * Construct a chunk that represents LITERAL.
 * \param index  The last position of input in bytes.
 * \param length Length in bytes.
 * \return Intermediate representation of LITERAL.
 */
Chunk Literal(size_t index, uint16_t length);

/**
 * Construct a chunk that represents COPY_FROM_LAST_OFFSET.
 * \param index  The last position of input in bytes.
 * \param offset The offset of the matched portion (should be same as the last offset).
 * \param length Length in bytes.
 * \return Intermediate representation of COPY_FROM_LAST_OFFSET.
 */
Chunk MatchFromLastOffset(size_t index, uint16_t offset, uint16_t length);

/**
 * Construct a chunk that represents COPY_FROM_NEW_OFFSET.
 * \param index  The last position of input in bytes.
 * \param offset The offset of the matched portion (should be differ from the last offset).
 * \param length Length in bytes.
 * \return Intermediate representation of COPY_FROM_NEW_OFFSET.
 */
Chunk MatchFromNewOffset(size_t index, uint16_t offset, uint16_t length);

/**
 * Tests if a chunk represents LITERAL or not.
 * \param c A chunk.
 * \return `true` if `c` represents LITERAL.
 */
bool is_Literal(Chunk c);

/**
 * Tests if a chunk represents COPY_FROM_LAST_OFFSET or not.
 * \param c A chunk.
 * \param last_offset The last offset value.
 * \return `true` if `c` represents COPY_FROM_LAST_OFFSET.
 */
bool is_MatchFromLastOffset(Chunk c, uint16_t last_offset);

/**
 * Tests if a chunk represents COPY_FROM_NEW_OFFSET or not.
 * \param c A chunk.
 * \param last_offset The last offset value.
 * \return `true` if `c` represents COPY_FROM_NEW_OFFSET.
 */
bool is_MatchFromNewOffset(Chunk c, uint16_t last_offset);

#endif // CHUNK_H_
