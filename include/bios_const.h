// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bios_const.h
 * \brief Constant variables in the MSX BIOS ROM.
 * \ingroup BIOS
 * \note This defines address, type, and name of variables.
 */

#pragma once

#ifndef BIOS_CONST_H
#define BIOS_CONST_H

#include <stdint.h>
#include "config.h"

/**
 * `MSX` Structure of MSX bitmap fonts (256 pattern x 8x8 pix x 1bpp).
 * \sa CGTBL
 * \sa CGPNT
 */
struct font {
  uint8_t data[256][8];
};

/**
 * `MSX` Pointer to MSX fonts in MAIN ROM (0004H / MAIN).
 * \sa CGPNT
 */
static __at (0x0004) const struct font * const CGTBL;

/**
 * `MSX` International identifier code #1 of MSX BIOS (002BH / MAIN).
 *
 * \sa MSX Datapack Volume 1, pp.322
 */
static __at (0x002b) const uint8_t INTERNATIONAL_ID_1;

/**
 * `MSX` International identifier code #2 of MSX BIOS (002CH / MAIN).
 *
 * \sa MSX Datapack Volume 1, pp.322
 */
static __at (0x002c) const uint8_t INTERNATIONAL_ID_2;

/**
 * `MSX` System version code of MSX BIOS (002DH / MAIN).
 *
 * The system version code written to address 002DH in MAIN ROM is as follows:
 *
 * | platform  | BASIC version | value at 002DH |
 * | --------- | ------------- | -------------- |
 * | MSX       | BASIC 1.xx    | `0`            |
 * | MSX2      | BASIC 2.xx    | `1`            |
 * | MSX2+     | BASIC 3.xx    | `2`            |
 * | MSXturboR | BASIC 4.xx    | `3`            |
 *
 * \sa MSX Datapack Volume 1, pp.322
 * \sa MSX Datapack Volume 3, pp.43
 */
static __at (0x002d) const uint8_t MSX_SYSTEM_VERSION_CODE;

#endif
