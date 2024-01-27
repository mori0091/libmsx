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
 * \file xmem.h
 * \brief Inter-memory transfer functions for ROM / RAM, VRAM, and Banked Memory (MegaROM).
 * \ingroup XMEM
 */
/**
 * \defgroup XMEM XMEM - Inter-memory transfer.
 * `#include <xmem.h>`
 */

#ifndef XMEM_H_
#define XMEM_H_

#include "bmem_rd.h"
#include "mem_rw.h"
#include "vmem_rw.h"

/**
 * \defgroup XMEM_INTER_MEMORY_TRANSFER Building blocks for inter-memory transfer.
 * \ingroup XMEM
 * Building blocks for copying and transferring data between different types of
 * memory devices.
 *
 * @{
 */

/**
 * Copy chunk from banked-memory to RAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `bmem_is_opened() && mem_is_opened()`
 */
void bmem_to_mem(size_t len);

/**
 * Copy chunk from banked-memory to RAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `bmem_is_opened() && mem_is_opened()`
 * \note This is a wrapper function of bmem_to_mem().
 */
void bmem_to_mem_u32(uint32_t len);

/**
 * Copy chunk from banked-memory to VRAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `bmem_is_opened() && vmem_is_opened()`
 * \note This is a wrapper function of bmem_to_vmem_u32().
 */
void bmem_to_vmem(size_t len);

/**
 * Copy chunk from banked-memory to VRAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `bmem_is_opened() && vmem_is_opened()`
 */
void bmem_to_vmem_u32(uint32_t len);

/**
 * Copy chunk from ROM/RAM to VRAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `mem_is_opened() && vmem_is_opened()`
 */
void mem_to_vmem(size_t len);

/**
 * Copy chunk from ROM/RAM to VRAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `mem_is_opened() && vmem_is_opened()`
 * \note This is a wrapper function of mem_to_vmem().
 */
void mem_to_vmem_u32(uint32_t len);

/**
 * Copy chunk from VRAM to ROM/RAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `vmem_is_opened() && mem_is_opened()`
 */
void vmem_to_mem(size_t len);

/**
 * Copy chunk from VRAM to ROM/RAM.
 *
 * \param len  size of chunk in bytes.
 *
 * \pre `vmem_is_opened() && mem_is_opened()`
 * \note This is a wrapper function of vmem_to_mem().
 */
void vmem_to_mem_u32(uint32_t len);

/** @} */

#endif // XMEM_H_
