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
 * \file VGMHeader.hpp
 */

#ifndef VGMHEADER_HPP_
#define VGMHEADER_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>

#include "io.hpp"

struct VGMHeader {
  void print();
  void parse(std::istream & is);

  uint32_t eof_offset;
  uint32_t version;
  uint32_t SN76489_clock;
  uint32_t YM2413_clock;
  uint32_t GD3_offset;
  uint32_t total_samples;
  uint32_t loop_offset;
  uint32_t loop_samples;
  // VGM 1.01 or later
  uint32_t rate;
  // VGM 1.10 or later
  uint16_t SN76489_feedback;
  uint8_t SN76489_shift_register_width;
  // VGM 1.51 or later
  uint8_t SN76489_flags;
  // VGM 1.10 or later
  uint32_t YM2612_clock;
  uint32_t YM2151_clock;
  // VGM 1.50 or later
  uint32_t VGM_data_offset;
  // VGM 1.51 or later
  uint32_t Sega_PCM_clock;
  uint32_t Sega_PCM_interface_register;
  uint32_t RF5C68_clock;
  uint32_t YM2203_clock;
  uint32_t YM2608_clock;
  uint32_t YM2610_clock;
  uint32_t YM3812_clock;
  uint32_t YM3526_clock;
  uint32_t Y8950_clock;
  uint32_t YMF262_clock;
  uint32_t YMF278B_clock;
  uint32_t YMF271_clock;
  uint32_t YMZ280B_clock;
  uint32_t RF5C164_clock;
  uint32_t PWM_clock;
  uint32_t AY8910_clock;
  uint8_t AY8910_chip_type;
  uint8_t AY8910_flags;
  uint8_t YM2203_AY8910_flags;
  uint8_t YM2608_AY8910_flags;
  // VGM 1.60 or later
  uint8_t volume_modifier;

  int8_t loop_base;
  // VGM 1.51 or later
  uint8_t loop_modifier;
  // VGM 1.61 or later
  uint32_t GameBoy_DMG_clock;
  uint32_t NES_APU_clock;
  uint32_t MultiPCM_clock;
  uint32_t uPD7759_clock;
  uint32_t OKIM6258_clock;
  uint8_t OKIM6258_flags;
  uint8_t K054539_flags;
  uint8_t C140_chip_type;

  uint32_t OKIM6295_clock;
  uint32_t K051649_K052539_clock; // SCC / SCC+
  uint32_t K054539_clock;
  uint32_t HuC6280_clock;
  uint32_t C140_clock;
  uint32_t K053260_clock;
  uint32_t Pokey_clock;
  uint32_t QSound_clock;
  // VGM 1.71 or later
  uint32_t SCSP_clock;
  // VGM 1.70 or later
  uint32_t extra_header_offset;
  // VGM 1.71 or later
  uint32_t WonderSwan_clock;
  uint32_t VSU_clock;
  uint32_t SAA1099_clock;
  uint32_t ES5503_clock;
  uint32_t ES5505_ES5506_clock;
  uint8_t ES5503_amount_of_output_channels;
  uint8_t ES5505_ES5506_amount_of_output_channels;
  uint8_t C352_clock_divider;
  uint32_t X1_010_clock;
  uint32_t C352_clock;
  uint32_t GA20_clock;
};

#endif // VGMHEADER_HPP_
