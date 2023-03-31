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
 * \file zx0.h
 */

#ifndef ZX0_H_
#define ZX0_H_

#include <stdbool.h>
#include <stdint.h>

#include "chunk_list.h"

#define INITIAL_OFFSET     (1)
#define MAX_OFFSET         (32640) // 32K - 128

extern uint16_t last_offset;

extern bool backtrack;
extern uint8_t bit_mask;
extern uint8_t * bit_value_ptr;
extern uint8_t * dst_ptr;

/**
 * Returns the upper limit of offset values that can be used at a given
 * position.
 * \param index Next position of output in bytes.
 * \return The upper limit of offset values.
 */
uint16_t max_offset_at(int index);

/**
 * Returns the number of bits of Elias Gamma.
 * \param x An integer value.
 * \return The number of bits in the corresponding Elias Gamma code.
 */
int bit_length_of_elias_gamma(int x);

/**
 * Construct a list of Chunk that represents optimal compression.
 * \param beg Pointer to the head of the input (inclusive).
 * \param end Pointer to the end of the input (out of range).
 * \return A list that represents optimal compression.
 */
List(Chunk) optimize(const uint8_t * const beg, const uint8_t * const end);

/**
 * Compress.
 * \param src Pointer to the head of input.
 * \param len Length in bytes.
 * \return Length of output in bytes.
 */
size_t compress(const uint8_t * src, size_t len);

#endif // ZX0_H_
