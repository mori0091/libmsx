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
 * \file scc_io.h
 */

#pragma once

#ifndef SCC_IO_H_
#define SCC_IO_H_

#include <config.h>
#include <stdint.h>

volatile static __at(0x5000) uint8_t  SCC_BANK_SELECT_0;
volatile static __at(0x7000) uint8_t  SCC_BANK_SELECT_1;
volatile static __at(0x9000) uint8_t  SCC_BANK_SELECT_2;
volatile static __at(0xb000) uint8_t  SCC_BANK_SELECT_3;

volatile static __at(0x9800) int8_t   SCC_waveform[4][32]; // channel 1..4
volatile static __at(0x9880) uint16_t SCC_fdr[5];
volatile static __at(0x988a) uint8_t  SCC_volume[5];
volatile static __at(0x988f) uint8_t  SCC_channel_mask;

volatile static __at(0x98a0) int8_t   SCC_waveform5[32]; // channel 5 (readonly)
volatile static __at(0x98e0) uint8_t  SCC_deformation;

volatile static __at(0xb800) int8_t   SCCPlus_waveform[5][32];
volatile static __at(0xb8a0) uint16_t SCCPlus_fdr[5];
volatile static __at(0xb8aa) uint8_t  SCCPlus_volume[5];
volatile static __at(0xb8af) uint8_t  SCCPlus_channel_mask;

volatile static __at(0xb8c0) uint8_t  SCCPlus_deformation;

volatile static __at(0xbffe) uint8_t  SCCPlus_mode_select;

#endif // SCC_IO_H_
