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
 * Decompress the ZX0 data stream.
 *
 * \param src  Pointer to ZX0 data stream.
 * \param dst  Destination base address.
 * \return     Length (in bytes) of the decompressed data.
 */
size_t ZX0_decompress(const uint8_t * src, uint8_t * dst);

#endif // ZX0_DECOMPRESS_H_
