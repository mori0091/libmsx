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
 * \file vmem_rw.h
 * \brief Stream like access functions for VRAM.
 * \ingroup VMEM_RW
 */

#ifndef VMEM_RW_H_
#define VMEM_RW_H_

#include "vmem.h"

#include <stdbool.h>

/**
 * \defgroup VMEM_RW A part of building blocks for inter-memory transfer, dedicated to VRAM access.
 * \ingroup XMEM
 * Building blocks for copying and transferring data between different types of
 * memory devices.
 * `#include <vmem_rw.h>`
 *
 * @{
 */

// A part of building blocks for heterogeneous memory architechture, dedicated
// to VRAM access.

/**
 * `MSX` Tests whether VRAM accessor is opened or not.
 *
 * \return whether VRAM accessor is opened or not.
 */
bool vmem_is_opened(void);

/**
 * `MSX` Open VRAM accessor.
 *
 * \param loc  base address.
 *
 * \pre `!vmem_is_opened()`
 */
void vmem_open(vmemptr_t loc);

/**
 * `MSX` Close VRAM accessor.
 *
 * \pre `vmem_is_opened()`
 */
void vmem_close(void);

/**
 * `MSX` Duplicate a byte sequence of VRAM starting from the specified offset to
 * current position and later.
 *
 * A data in range of `[p - offset, p - offset + len)` of VRAM will be copied to
 * `[p, p + len)`, where `p` is current position. If both area overlaps,
 * repetition is occur.
 *
 * For example, if the byte in `p-1` is `X`, then `vmem_dup(1, 3)` copies `X` to
 * `p`, `p+1`, `p+2` and `p` is increased by `3`.
 *
 * \param offset distance from the current position.
 * \param len    length in bytes.
 *
 * \pre `vmem_is_opened()`
 */
void vmem_dup(uint16_t offset, size_t len);

// internal functions

/**
 * Get current position of VRAM accessor.
 *
 * \return current position.
 *
 * \pre vmem_is_opened()
 *
 * \note internal use only.
 */
vmemptr_t vmem_ptr(void);

/**
 * Advance the current position of VRAM accessor.
 *
 * \param len   number of bytes.
 *
 * \pre vmem_is_opened()
 */
void vmem_skip(size_t len);

/**
 * Write a chunk to VRAM.
 *
 * \param src     source base address.
 * \param len     number of bytes to be copied.
 *
 * \pre vmem_set_write_address() has been called.
 * \pre vmem_set_read_address() has not been called after vmem_set_write_address().
 *
 * \note internal use only.
 */
void vmem_write_chunk(const uint8_t * src, uint16_t len);

/**
 * Read a chunk from VRAM.
 *
 * \param dst     destination base address.
 * \param len     number of bytes to be copied.
 *
 * \pre vmem_set_read_address() has been called.
 * \pre vmem_set_write_address() has not been called after vmem_set_read_address().
 *
 * \note internal use only.
 */
void vmem_read_chunk(uint8_t * dst, uint16_t len);

/** @} */

#endif // VMEM_RW_H_
