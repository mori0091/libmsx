// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2026 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file io.h
 * \brief Definition of I/O ports.
 */

#pragma once

#ifndef IO_H
#define IO_H

#include "config.h"

// ---- VDP I/O ports
MSX_IO_PORT(0x98) vdp_port0;
MSX_IO_PORT(0x99) vdp_port1;
MSX_IO_PORT(0x9a) vdp_port2;
MSX_IO_PORT(0x9b) vdp_port3;

// ---- PSG I/O ports
MSX_IO_PORT(0x0a0) psg_port0; /* address latch */
MSX_IO_PORT(0x0a1) psg_port1; /* data write */
MSX_IO_PORT(0x0a2) psg_port2; /* data read */

// ---- Slot I/O ports
MSX_IO_PORT(0x0a8) primary_slot_select_port;

// ---- Kanji ROM I/O ports
// JIS X 0208 第1水準 漢字ROM
MSX_IO_PORT(0x0d8) KANJI_ROM_JIS1_port1; /* Read font / Write LSB 6 bits of kanji code */
MSX_IO_PORT(0x0d9) KANJI_ROM_JIS1_port2; /* Read font / Write MSB 6 bits of kanji code */
// JIS X 0208 第2水準 漢字ROM
MSX_IO_PORT(0x0da) KANJI_ROM_JIS2_port1; /* Read font / Write LSB 6 bits of kanji code */
MSX_IO_PORT(0x0db) KANJI_ROM_JIS2_port2; /* Read font / Write MSB 6 bits of kanji code */

#endif
