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
 * \file vmem.h
 * \brief Data types and functions for VRAM access.
 * \ingroup VMEM
 */
/**
 * \defgroup VMEM VRAM - Video RAM
 * `#include <vmem.h>`
 */

#pragma once

#ifndef VMEM_H
#define VMEM_H

#include <stdint.h>
#include <stddef.h>

#include "io.h"

// ---- VRAM access
/**
 * \defgroup VMEM_ACCESS VRAM access interface
 * \ingroup VMEM
 * Read / Write VRAM.
 * @{
 */

/**
 * Type for VRAM address.
 */
typedef uint32_t vmemptr_t;

/**
 * `MSX` Set VRAM address for sequential reading of VRAM.
 *
 * This function sets the VRAM access base address register (R#14) and/or the
 * VRAM address counter to a value based on `loc` for sequential reading of VRAM
 * in subsequent calls to `vmem_get()`.
 *
 * \param loc  VRAM address.
 *
 * \sa vmem_get()
 */
void vmem_set_read_address(vmemptr_t loc);

/**
 * `MSX` Set VRAM address for sequential writing of VRAM.
 *
 * This function sets the VRAM access base address register (R#14) and/or the
 * VRAM address counter to a value based on `loc` for sequential writing of VRAM
 * in subsequent calls to `vmem_set()`.
 *
 * \param loc  VRAM address.
 *
 * \sa vmem_set()
 */
void vmem_set_write_address(vmemptr_t loc);

/**
 * `MSX` Read 1 byte from VRAM.
 *
 * \return a value read from VRAM.
 *
 * \note
 * vmem_set_read_address() must be called at least once, before calling
 * vmem_get(). Also, after the last call to vmem_set_read_address() and before
 * calling vmem_get(), vmem_set_write_address() must not be called.
 *
 * \sa vmem_set_read_address()
 * \sa vmem_read()
 */
inline uint8_t vmem_get(void) {
  return vdp_port0;
}

/**
 * `MSX` Write 1 byte to VRAM.
 *
 * \param val  a value to be written to VRAM.
 *
 * \note
 * vmem_set_write_address() must be called at least once, before calling
 * vmem_set(). Also, after the last call to vmem_set_write_address() and before
 * calling vmem_set(), vmem_set_read_address() must not be called.
 *
 * \sa vmem_set_write_address()
 * \sa vmem_write()
 */
inline void vmem_set(uint8_t val) {
  vdp_port0 = val;
}

/**
 * `MSX` Read a series of bytes from VRAM.
 *
 * This function copies `len` bytes from VRAM to RAM.
 *
 * That is almost same as the below code:
 * ~~~
 * vmem_set_read_address(src);
 * for (uint8_t *p = dst; len--; ) {
 *   *p++ = vmem_get();
 * }
 * ~~~
 *
 * \param src  source base address of VRAM.
 * \param dst  destination base address of RAM.
 * \param len  number of bytes to be read.
 */
void vmem_read(vmemptr_t src, void* dst, uint16_t len);

/**
 * `MSX` Write a series of bytes to VRAM.
 *
 * This function copies `len` bytes from ROM/RAM to VRAM.
 *
 * That is almost same as the below code:
 * ~~~
 * vmem_set_write_address(dst);
 * for (uint8_t *p = src; len--; ) {
 *   vmem_set(*p++);
 * }
 * ~~~
 *
 * \param dst  destination base address of VRAM.
 * \param src  source base address of ROM/RAM.
 * \param len  number of bytes to be written.
 */
void vmem_write(vmemptr_t dst, void* src, uint16_t len);

/**
 * `MSX` Fill a series of bytes in VRAM with the specified value.
 *
 * That is almost same as the below code:
 * ~~~
 * vmem_set_write_address(dst);
 * for (; len--; ) {
 *   vmem_set(val);
 * }
 * ~~~
 *
 * \param dst  destination base address of VRAM.
 * \param val  a byte value to be written.
 * \param len  number of bytes to be written.
 */
void vmem_memset(vmemptr_t dst, uint8_t val, uint16_t len);

/**
 * `MSX` Copy a series of bytes in VRAM (VRAM to VRAM).
 *
 * VRAM to VRAM copying function.
 *
 * This function uses a preconfigured built-in working buffer.
 *
 * \param src  source base address of VRAM.
 * \param dst  destination base address of VRAM.
 * \param len  number of bytes to be copied.
 *
 * \sa
 * vmem_copy_b() for using user-defined working buffer instead of built-in
 * buffer.
 */
void vmem_copy(vmemptr_t src, vmemptr_t dst, uint32_t len);

/**
 * `MSX2` Copy a series of bytes in VRAM (VRAM to VRAM).
 *
 * Optimal version of vmem_copy().
 *
 * This function does not use built-in or user-defined buffers. Instead, it uses
 * page #2 of the main RAM (usually unused RAM in ROM cartridge applications) as
 * a work buffer. This provides better performance in terms of speed and memory
 * consumption.
 *
 * \param src  source base address of VRAM.
 * \param dst  destination base address of VRAM.
 * \param len  number of bytes to be copied.
 *
 * \note
 * Page #2 (`0x8000`..`0xbfff`) of the main RAM is used as a 16KiB buffer.
 * Therefore, at least 32 KiB of main RAM is required (Page #2 + Page #3 = 16
 * KiB + 16 KiB). Some MSX computers do not meet this requirement, but MSX2 and
 * later do. MSX1 is also acceptable if it has at least 32 KiB of RAM.
 *
 * \attention
 * While executing this function, main RAM is exposed on page #2.
 * Therefore, direct access to page #2 ROM is prohibited until the function call
 * is completed. For example, if an interrupt routine accesses page #2 without
 * using an inter-slot call, its behavior is undefined.
 */
void vmem_copy_opt2(vmemptr_t src, vmemptr_t dst, uint32_t len);

/**
 * `MSX` Copy a series of bytes in VRAM (VRAM to VRAM).
 *
 * Primitive function for copying from VRAM to VRAM. The work buffer must be
 * specified by the caller.
 *
 * - Processing time depends on the size of the work buffer.
 * - Larger buffer size is better (faster).
 * - If the size is very small, this function will be very slow.
 * - The recommended size is from `256` to `1024` bytes (or more if available).
 *
 * \param src     source base address of VRAM.
 * \param dst     destination base address of VRAM.
 * \param len     number of bytes to be copied.
 * \param buf     base address of working buffer.
 * \param buf_len capacity of the working buffer, in bytes.
 */
void vmem_copy_b(vmemptr_t src, vmemptr_t dst, uint32_t len, uint8_t * buf, size_t buf_len);

/** @} */

#endif
