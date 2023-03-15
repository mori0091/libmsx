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
 * \file ZX0_decompress.h
 *
 * Decompresser of ZX0 compressed file format version 2.
 *
 * The ZX0 data compression format and algorithm was designed by Einar Saukas.
 *
 * This C language implementation of ZX0 decompresser is reimplemented by Daishi
 * Mori (mori0091) for use with libmsx.
 *
 * \sa https://github.com/einar-saukas/ZX0
 */

#ifndef ZX0_DECOMPRESS_H_
#define ZX0_DECOMPRESS_H_

#include "bmem.h"
#include "vmem.h"

#include <stdint.h>
#include <stddef.h>

/**
 * Decompress a ZX0 data stream.
 *
 * \param src  Pointer to ZX0 data stream.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
size_t ZX0_decompress(const uint8_t * src, uint8_t * dst);

/**
 * Decompress a ZX0 data stream in Banked Memory.
 *
 * \param src  Pointer to ZX0 data stream in Banked Memory.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
size_t ZX0_decompress_bmem(bmemptr_t src, uint8_t * dst);

/**
 * Decompress a ZX0 data stream in Banked Memory to VRAM.
 *
 * \param src  Pointer to ZX0 data stream in Banked Memory.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
uint32_t ZX0_decompress_bmem_to_vmem(bmemptr_t src, vmemptr_t dst);

/**
 * Decompress a ZX0 resource in banked memory to VRAM.
 *
 * Searches for an embedded resource in banked memory by name and decmompress it
 * into the specified address in VRAM. If the resource is not found, do nothing.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   ZX0_decompress_bmem_to_vmem(res->offset, dst);
 * }
 * ~~~
 *
 * \param path path/file name of the resource.
 * \param dst destination address of VRAM.
 *
 * \sa ZX0_decompress_bmem_to_vmem()
 */
uint32_t ZX0_decompress_resource_to_vmem(const char * path, vmemptr_t dst);

#endif // ZX0_DECOMPRESS_H_
