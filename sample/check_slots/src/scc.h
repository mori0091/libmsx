// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file scc.h
 */

#pragma once

#ifndef SCC_H_
#define SCC_H_

#include <stdbool.h>
#include <stdint.h>

#include <config.h>

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

struct SCC_channel {
  void (* read_waveform)(int8_t data[32]);
  void (* write_waveform)(const int8_t data[32]);
  void (* read_fdr)(uint16_t * fdr);
  void (* write_fdr)(const uint16_t fdr);
  void (* read_volume)(uint8_t * volume);
  void (* write_volume)(const uint8_t volume);
};
struct SCC {
  uint8_t slot;
  uint8_t version;              // 1: SCC, 2: SCC+
  uint8_t (* read_channel_mask)(void);
  void (* write_channel_mask)(const uint8_t mask);
  void (* write_deformation)(const uint8_t value);
  struct SCC_channel channels[5];
};

uint8_t SCC_inspect(uint8_t slot);
void SCC_find(struct SCC * scc);

#endif // SCC_H_
