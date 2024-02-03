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
 * \file BitReader.h
 */

#ifndef BITREADER_H_
#define BITREADER_H_

#include <stdbool.h>
#include <stdint.h>

#include <memfile.h>

/**
 * Context for reading bit-string and/or octet-string from an input stream.
 */
struct BitReader {
  uint8_t bitmask;         ///< Mask for extract the next bit.
  uint8_t bits;            ///< Cache of bit-string for extracting the next bit.
  MemFile mf;              ///< Memory-file pointer of compressed input stream.
};

/**
 * Read next bit from stream.
 *
 * \param in   Pointer to a BitReader.
 * \return the next bit read.
 */
bool read_bit(struct BitReader * in);

/**
 * Count the number of consecutive `0` bits.
 *
 * \param in   Pointer to a BitReader.
 * \return the number of consecutive `0` bits.
 */
uint8_t count_zeros(struct BitReader * in);

/**
 * Read Elias-Gamma code.
 *
 * Read an Elias-Gamma code from the input stream, then decode it to a positive
 * integer.
 *
 * \param in   Pointer to a BitReader.
 * \return The decoded positive integer.
 *
 * \note
 * If the decoded integer is greater than `255`, the return value is undefined.
 */
uint8_t elias_gamma_u8(struct BitReader * in);

/**
 * Read Elias-Gamma code.
 *
 * Read an Elias-Gamma code from the input stream, then decode it to a positive
 * integer.
 *
 * \param in   Pointer to a BitReader.
 * \return The decoded positive integer.
 *
 * \note
 * If the decoded integer is greater than `UINT32_MAX`, the return value is
 * undefined.
 */
uint32_t elias_gamma_u32(struct BitReader * in);

#endif // BITREADER_H_
