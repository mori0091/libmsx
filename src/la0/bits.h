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
 * \file bits.h
 */

#ifndef BITS_H_
#define BITS_H_

#include <stdint.h>

/**
 * Count the number of consecutive `0` bits.
 *
 * \param x    a bit-string.
 * \return the number of consecutive `0` bits.
 */
uint8_t bits_count_leading_zeros_u8(uint8_t x);

#endif // BITS_H_
