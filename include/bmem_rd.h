// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem_rd.h
 * \brief Stream like access functions for Banked Memory.
 * \ingroup BMEM_RD
 */

#ifndef BMEM_RD_H_
#define BMEM_RD_H_

#include "bmem.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * \defgroup BMEM_RD A part of building blocks for inter-memory transfer, dedicated to Banked Memory access.
 * \ingroup XMEM
 * Building blocks for copying and transferring data between different types of
 * memory devices.
 * `#include <bmem_rd.h>`
 *
 * @{
 */

/**
 * `MSX` Tests whether Banked Memory accessor is opened or not.
 *
 * \return whether Banked Memory accessor is opened or not.
 */
bool bmem_is_opened(void);

/**
 * `MSX` Open Banked Memory accessor.
 *
 * \param loc  base address.
 *
 * \pre `!bmem_is_opened()`
 */
void bmem_open(bmemptr_t loc);

/**
 * `MSX` Close Banked Memory accessor.
 *
 * \pre `bmem_is_opened()`
 */
void bmem_close(void);

/**
 * Return the size of remaining bytes in the current segment.
 *
 * \return the size of remaining bytes in the current segment.
 *
 * \pre bmem_is_opened()
 */
size_t bmem_avail(void);

/**
 * Ensure the internal pointer points the current position.
 *
 * \pre bmem_is_opened()
 */
void bmem_ensure(void);

/**
 * Get the internal pointer value of Banked Memory accessor.
 *
 * \return the internal pointer value.
 *
 * \pre bmem_is_opened()
 */
const uint8_t * bmem_ptr(void);

/**
 * Advance the current position of Banked Memory accessor.
 *
 * \param len   number of bytes.
 *
 * \pre bmem_is_opened()
 */
void bmem_skip(size_t len);

/**
 * Switch to next segment.
 *
 * \pre bmem_is_opened()
 */
void bmem_next_segment(void);

/**
 * Read a chunk from Banked Memory.
 *
 * \param dst     destination base address.
 * \param len     number of bytes to be copied.
 *
 * \pre `bmem_is_opened()`
 */
void bmem_read_chunk(uint8_t * dst, size_t len);

/**
 * Read an octet from Banked Memory.
 *
 * \return a value read from Banked Memory.
 *
 * \pre `bmem_is_opened()`
 */
uint8_t bmem_read_byte(void);

/** @} */

#endif // BMEM_RD_H_
