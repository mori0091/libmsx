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
 * \file mem_rw.h
 * \brief Stream like access functions for ROM / RAM.
 * \ingroup MEM_RW
 */

#ifndef MEM_RW_H_
#define MEM_RW_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * \defgroup MEM_RW A part of building blocks for inter-memory transfer, dedicated to ROM / RAM access.
 * \ingroup XMEM
 * Building blocks for copying and transferring data between different types of
 * memory devices.
 * `#include <mem_rw.h>`
 *
 * @{
 */

/**
 * Tests whether ROM/RAM accessor is opened or not.
 *
 * \return whether ROM/RAM accessor is opened or not.
 */
bool mem_is_opened(void);

/**
 * Open ROM/RAM accessor.
 *
 * \param loc  base address.
 *
 * \pre `!mem_is_opened()`
 */
void mem_open(uint8_t * loc);

/**
 * Close ROM/RAM accessor.
 *
 * \pre `mem_is_opened()`
 */
void mem_close(void);

/**
 * Duplicate a byte sequence starting from the specified offset to current
 * position and later.
 *
 * A data in range of `[p - offset, p - offset + len)` of RAM/ROM will be copied
 * to `[p, p + len)`, where `p` is current position. If both area overlaps,
 * repetition is occur.
 *
 * For example, if the byte in `p-1` is `X`, then `mem_dup(1, 3)` copies `X` to
 * `p`, `p+1`, `p+2` and `p` is increased by `3`.
 *
 * \param offset distance from the current position.
 * \param length length in bytes.
 *
 * \pre `mem_is_opened()`
 */
void mem_dup(uint16_t offset, size_t len);

/**
 * Get current position of ROM/RAM accessor.
 *
 * \return current position.
 *
 * \pre `mem_is_opened()`
 */
uint8_t * mem_ptr(void);

/**
 * Tests whether the specified range is in free area.
 *
 * \param p   base address of the range.
 * \param len length the range in bytes.
 * \return `true` if the range is in [`0xc000`, `(HIMEM)`), otherwise `false`.
 *
 * \note
 * This function just checks that the specified area is in range of free area.
 * No data is written to the memory.
 *
 * \note
 * Page #`0`..#`2` (`0x0000`..`0xbfff`) are ROM.
 *
 * \note
 * `HIMEM` points the lowest address of reserved area. (out of free area)
 *
 * \note
 * DATA segment, HEAP, and stack area are in the free area.
 *
 * \note
 * Address `0xffff` is not memory, that is "extended slot selector" register.
 *
 * \attention
 * When writing to memory, the stack area or work area is overwritten (broken)
 * if it intersects the destination area. Unfortunately, however, the library
 * cannot determine the proper boundaries. This must be addressed by the
 * application programmer.
 */
bool mem_is_in_free_area(uint8_t * p, size_t len);

/**
 * Write a chunk to RAM.
 *
 * \param src     source base address.
 * \param len     number of bytes to be copied.
 *
 * \pre `mem_is_opened()`
 *
 * \note
 * The destination [`mem`, `mem+len`) must be in range of [`0xc000`, `(HIMEM)`),
 * where `mem` is the current position.
 */
void mem_write_chunk(const uint8_t * src, size_t len);

/**
 * Read a chunk from ROM/RAM.
 *
 * \param dst     destination base address.
 * \param len     number of bytes to be copied.
 *
 * \pre `mem_is_opened()`
 *
 * \note
 * The destination [`dst`, `dst+len`) must be in range of [`0xc000`, `(HIMEM)`).
 */
void mem_read_chunk(uint8_t * dst, size_t len);

/**
 * Write an octet to RAM.
 *
 * \param x a value to be written to RAM.
 *
 * \note
 * The current position must be in range of [`0xc000`, `(HIMEM)`).
 */
void mem_write_byte(uint8_t x);

/**
 * Read an octet from ROM/RAM.
 *
 * \return a value read from ROM/RAM.
 *
 * \pre `mem_is_opened()`
 */
uint8_t mem_read_byte(void);

/** @} */

#endif // MEM_RW_H_
