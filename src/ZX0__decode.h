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
 * \file ZX0__decode.h
 */

#ifndef ZX0__DECODE_H_
#define ZX0__DECODE_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Function pointer to a `duplicate` function.
 *
 * This function pointer must be set before calling to ZX0__decode().
 *
 * `duplicate` function shall duplicates a sequence of bytes beginning at the
 * specified offset in the output stream at the current position or later.
 *
 * A data in range of `[p - offset, p - offset + len)` of output stream will be
 * copied to `[p, p + len)`, where `p` is current position. If both area
 * overlaps, repetition shall be occur.
 *
 * For example, if the byte in `p-1` is `X`, then `duplicate(1, 3)` copies `X`
 * to `p`, `p+1`, `p+2` and `p` is increased by `3`.
 *
 * \param offset distance from the current position.
 * \param length length in bytes.
 */
extern void (* ZX0__fnptr_duplicate)(uint16_t offset, size_t len);

/**
 * Function pointer to a `transfer` function.
 *
 * This function pointer must be set before calling to ZX0__decode().
 *
 * `transfer` function shall copy chunk of specified length taken from input
 * stream to output stream. Current position of input and output stream shall be
 * advanced by the specified length.
 *
 * \param len  size of chunk in bytes.
 */
extern void (* ZX0__fnptr_transfer)(size_t len);

/**
 * Function pointer to a `read_byte` function.
 *
 * This function pointer must be set before calling to ZX0__decode().
 *
 * `read_byte` function shall take an octet from input stream. Current position
 * of input stream shall be advanced by 1.
 *
 * \return a value read from input stream.
 */
extern uint8_t (* ZX0__fnptr_read_byte)(void);

/**
 * Decompress ZX0 input stream to output stream.
 *
 * Before calling this function, the current location of the input/output stream
 * and the ZX0__fnptr_duplicate, ZX0__fnptr_transfer, and ZX0__fnptr_read_byte
 * function pointers must be set. This function does not care about input/output
 * details such as the type or location of the input/output stream.
 */
void ZX0__decode(void);

#endif // ZX0__DECODE_H_
