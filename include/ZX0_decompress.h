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
 * \file ZX0_decompress.h
 *
 * Decompressor of the ZX0 data compression format version 2.
 *
 * The ZX0 data compression format and algorithm was designed by Einar Saukas.
 *
 * This C language implementation of ZX0 decompressor is reimplemented by Daishi
 * Mori (mori0091) for use with libmsx.
 *
 * \sa https://github.com/einar-saukas/ZX0
 */
/**
 * \defgroup ZX0 ZX0 data compression
 * `#include <ZX0_decompress.h>`
 */

#ifndef ZX0_DECOMPRESS_H_
#define ZX0_DECOMPRESS_H_

#include "bmem.h"
#include "vmem.h"

#include <stdint.h>
#include <stddef.h>

/**
 * \defgroup ZX0_DECOMPRESS ZX0 decompressor.
 * \ingroup ZX0
 * Decompressor of the ZX0 data compression format version 2.
 *
 * @{
 */

/**
 * `MSX` Decompress a ZX0 data stream.
 *
 * \param src  Pointer to ZX0 data stream.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
size_t ZX0_decompress(const uint8_t * src, uint8_t * dst);

/**
 * `MSX` Decompress a ZX0 data stream in Banked Memory.
 *
 * \param src  Pointer to ZX0 data stream in Banked Memory.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
size_t ZX0_decompress_bmem(bmemptr_t src, uint8_t * dst);

/**
 * `MSX` Decompress a ZX0 data stream in Banked Memory to VRAM.
 *
 * \param src  Pointer to ZX0 data stream in Banked Memory.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
uint32_t ZX0_decompress_bmem_to_vmem(bmemptr_t src, vmemptr_t dst);

/**
 * `MSX2` Decompress a ZX0 data stream in Banked Memory to VRAM via main RAM page #0 buffer.
 *
 * High throughput (but low responsiveness) version of ZX0_decompress_bmem_to_vmem().
 *
 * This function uses main RAM page #0 as 16KiB buffer for faster decompression.
 *
 * \param src  Pointer to ZX0 data stream in Banked Memory.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 *
 * \note
 * This is available for MSX1 w/ 64KiB main RAM if main RAM resides in same
 * slot.
 *
 * \note
 * But some MSX1 has only 8, 16, or 32 KiB main RAM. Or even if it has 64KiB
 * main RAM, some part of main RAM may resides in different slot. This function
 * does not support such MSX1. So it is described as for `MSX2` or later.
 */
uint32_t ZX0_decompress_bmem_to_vmem_buffer(bmemptr_t src, vmemptr_t dst);

/**
 * `MSX` Decompress a ZX0 resource in banked memory to VRAM.
 *
 * Searches for an embedded resource in banked memory by name and decmompress it
 * into the specified address in VRAM. If the resource is not found, do nothing.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   return ZX0_decompress_bmem_to_vmem(res->offset, dst);
 * }
 * return 0;
 * ~~~
 *
 * \param path path/file name of the resource.
 * \param dst destination address of VRAM.
 * \return size of decompressed image.
 *
 * \sa ZX0_decompress_bmem_to_vmem()
 */
uint32_t ZX0_decompress_resource_to_vmem(const char * path, vmemptr_t dst);

/**
 * `MSX2` Decompress a ZX0 resource in banked memory to VRAM via main RAM page #0 buffer.
 *
 * High throughput (but low responsiveness) version of ZX0_decompress_resource_to_vmem().
 *
 * This function uses main RAM page #0 as 16KiB buffer for faster decompression.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   return ZX0_decompress_bmem_to_vmem_buffer(res->offset, dst);
 * }
 * return 0;
 * ~~~
 *
 * \param path path/file name of the resource.
 * \param dst destination address of VRAM.
 * \return size of decompressed image.
 *
 * \note
 * This is available for MSX1 w/ 64KiB main RAM if main RAM resides in same
 * slot.
 *
 * \note
 * But some MSX1 has only 8, 16, or 32 KiB main RAM. Or even if it has 64KiB
 * main RAM, some part of main RAM may resides in different slot. This function
 * does not support such MSX1. So it is described as for `MSX2` or later.
 *
 * \sa ZX0_decompress_bmem_to_vmem_buffer()
 */
uint32_t ZX0_decompress_resource_to_vmem_buffer(const char * path, vmemptr_t dst);

/** @} */

#endif // ZX0_DECOMPRESS_H_
