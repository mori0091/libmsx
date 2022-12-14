// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem.h
 * \brief Data types and functions for accessing banked memory.
 * \ingroup BANKED_MEMORY
 */
/**
 * \defgroup BANKED_MEMORY Banked Memory interface for MegaROM.
 * `#include <bmem.h>`
 *
 * **MegaROM** is a ROM consisting of up to 256 8KiB or 16KiB segments.
 *
 * In `libmsx`, **MegaROM** is treated as **banked memory** consisting of one
 * large address space (or a series of 16KiB segments).
 *
 * The first 16KiB segment is used as the `CODE` segment.
 *
 * Subsequent segments are named `BANK1`, `BANK2`, ..., and `BANK63`.
 *
 * The `CODE` segment is always placed on page 1 (0x4000..0x7fff) of its slot's
 * address space.
 *
 * Page 2 (0x8000..0xbfff) is used as a switchable memory bank, and arbitrary
 * 16KiB segment in **banked memory** can be placed on page 2.
 *
 * Functions marked `__banked` in banked memory can be called from `CODE`
 * segments or other segments, see also SDCC Compiler User Manual "Bank
 * Switching" and "Z80 banked calls".
 *
 * `bmem.h` provides other functions to use banked memory as data storage.
 * - \ref BANKED_MEMORY_DIRECT_ACCESS
 * - \ref BANKED_MEMORY_INDIRECT_ACCESS
 *
 * `resources.h` provides functions to use banked memory as data storage of named
 * resources.
 * - \ref RESOURCES_FIND
 * - \ref RESOURCES_INDIRECT_ACCESS
 *
 * \note
 * These functions are available only when the `CONFIG_ROM_TYPE` is `ascii8` or
 * `ascii16`.
 *
 * @{
 */

#pragma once

#include <stdint.h>
#ifndef BMEM_H_
#define BMEM_H_

#include <vmem.h>

/**
 * \defgroup BANKED_MEMORY_DIRECT_ACCESS Direct access to banked memory by switching page 2.
 * \ingroup BANKED_MEMORY
 * APIs for direct access to 16KiB segments of banked memory.
 *
 * @{
 */

/**
 * Type of an address of banked memory.
 */
typedef uint32_t bmemptr_t;

/**
 * Returns the segment number corresponding to the specified address of the
 * banked memory.
 *
 * \return the segment number corresponding to `loc`.
 */
inline uint8_t bmem_bank_of(bmemptr_t loc) {
  return (uint8_t)((loc >> 14) & 255);
}

/**
 * Get the current segment number of banked memory at page 2 (0x8000..0xbfff).
 *
 * \return the current segment number.
 */
uint8_t bmem_get_bank(void) __naked;

/**
 * Expose a 16KiB segment of banked memory at page 2 (0x8000..0xbfff).
 *
 * \param bank the segment number.
 */
void bmem_set_bank(uint8_t bank) __naked;

/** @} */

/**
 * \defgroup BANKED_MEMORY_INDIRECT_ACCESS Reading and copying from banked memory.
 * \ingroup BANKED_MEMORY
 * APIs for indirect access to banked memory.
 *
 * Banked memory is treated as a ROM with one large address space to read and
 * copy data.
 *
 * @{
 */

/**
 * Read byte from banked memory.
 *
 * \param src address of banked memory.
 * \return a byte value.
 */
uint8_t bmem_get(bmemptr_t src);

/**
 * Read 16-bits value from banked memory.
 *
 * \param src address of banked memory.
 * \return a 16-bits value.
 */
uint16_t bmem_get_u16(bmemptr_t src);

/**
 * Read byte sequence from banked memory.
 *
 * \param src source address of banked memory.
 * \param dst destination address.
 * \param len number of bytes to be read.
 *
 * \note
 * This function is supposed to copy from banked memory to page 3 of main RAM.
 * Thus the destination area specified by `dst` and `len` must be in range of
 * `0xc000` to `0xfffe`. In `libmsx`, the `DATA` segment and stack areas are
 * placed on page 3 so this is reasonable.
 *
 * \note
 * In particular, page 2 is used to access banked memory so the destination
 * areas cannot be in page 2. Page 0 is MAIN ROM, page 1 is `CODE` segment, and
 * the library code itself is included in `CODE` segment. So these areas cannot
 * be specified as the destination areas.
 *
 * \note
 * Address `0xffff` is not memory, that is "extended slot selector" register.
 *
 * \attention
 * Stack areas and work areas are overridden if they intersect the destination
 * area specified by `dst` and `len`. Unfortunately, however, the library cannot
 * determine the appropriate bounds. The application programmer must deal with
 * this.
 */
void bmem_read(bmemptr_t src, void* dst, uint16_t len);

/**
 * Copy from banked memory to VRAM.
 *
 * \param src source address of banked memory.
 * \param dst destination address of VRAM.
 * \param len number of bytes to be copied.
 */
void bmem_copy_to_vmem(bmemptr_t src, vmemptr_t dst, uint32_t len);

/**
 * Load a `BSAVE` formatted binary in banked memory into VRAM.
 *
 * A `BSAVE` formatted binary must consist of a 7-byte header followed by the
 * data body, as follows:
 *
 * | address       | contents                   |
 * |---------------|----------------------------|
 * | `src+0`       | `0xFE`                     |
 * | `src+1`       | lo-byte of `start` address |
 * | `src+2`       | hi-byte of `start` address |
 * | `src+3`       | lo-byte of `end` address   |
 * | `src+4`       | hi-byte of `end` address   |
 * | `src+5`       | lo-byte of `run` address   |
 * | `src+6`       | hi-byte of `run` address   |
 * | `src+7`       | `body[0]`                  |
 * | ...           | ...                        |
 * | `src+7 + N-1` | `body[N-1]`                |
 *
 * where `N == end - start + 1`, and `start <= end`.
 *
 * `start` and `end` indicate the range of VRAM addresses where the image was;
 * for VRAM images, `run` is not used.
 *
 * This function copies the data body to `start`..`end` of VRAM.
 *
 * \param src address of `BSAVE` foramtted binary in banked memory.
 */
void bmem_bload_s(bmemptr_t src);

/** @} */

#endif // BMEM_H_

/** @} */
