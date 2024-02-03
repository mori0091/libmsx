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
 * \file VGMHeader.cpp
 */

#include "VGMHeader.hpp"

void VGMHeader::print() {
  std::cout << "VGM version : 0x" << std::hex << version << std::dec << std::endl;
  if (AY8910_clock) {
    std::cout << "PSG (AY-3-8910) clock : " << AY8910_clock << std::endl;
  }
  if (K051649_K052539_clock & (1 << 31)) {
    std::cout << "SCC+ (K052539) clock : " << (K051649_K052539_clock & 0x7fffffff) << std::endl;
  }
  else if (K051649_K052539_clock) {
    std::cout << "SCC  (K051649) clock : " << K051649_K052539_clock << std::endl;
  }
  if (YM2413_clock) {
    std::cout << "OPLL (YM2413) clock : " << YM2413_clock << std::endl;
  }
  {
    std::cout << "Total number of samples : " << total_samples << " @ 44.1kHz" << std::endl;
  }
  if (loop_samples) {
    std::cout << "Loop number of samples : " << loop_samples << " @ 44.1kHz" << std::endl;
  }
  if (rate) {
    std::cout << "Rate : " << rate << "Hz" << std::endl;
  }
  else {
    std::cout << "Rate : (unknown)" << std::endl;
  }
  {
    std::cout << "VGM data offset : " << VGM_data_offset << std::endl;
  }
  if (loop_offset) {
    std::cout << "Loop offset : " << loop_offset << std::endl;
  }
  if (GD3_offset) {
    std::cout << "GD3 tags offset : " << GD3_offset << std::endl;
  }
  std::cout << "EoF offset : " << eof_offset << std::endl;
}

void VGMHeader::parse(std::istream & is) {
  char buf[256] = {0};
  is.read(buf, 0x40);
  if (strncmp("Vgm ", buf, 4)) {
    std::cerr << "Illegal input." << std::endl;
    exit(1);
  }
  const char * p = buf;
  eof_offset    = u32(p + 0x04) + 0x04;
  version       = u32(p + 0x08);
  SN76489_clock = u32(p + 0x0C);
  YM2413_clock  = u32(p + 0x10);
  GD3_offset    = u32(p + 0x14);
  if (GD3_offset) {
    GD3_offset += 0x14;
  }
  total_samples = u32(p + 0x18);
  loop_offset   = u32(p + 0x1C);
  if (loop_offset) {
    loop_offset += 0x1C;
  }
  loop_samples  = u32(p + 0x20);
  // VGM 1.01 or later
  if (0x101 <= version) {
    rate = u32(p + 0x24);
  }
  // VGM 1.10 or later
  if (0x110 <= version) {
    SN76489_feedback             = u16(p + 0x28);
    SN76489_shift_register_width =  u8(p + 0x2A);
  }
  // VGM 1.51 or later
  if (0x151 <= version) {
    SN76489_flags = u8(p + 0x2B);
  }
  // VGM 1.10 or later
  if (0x110 <= version) {
    YM2612_clock = u32(p + 0x2C);
    YM2151_clock = u32(p + 0x30);
  }
  // VGM 1.50 or later
  if (0x150 <= version) {
    VGM_data_offset = u32(p + 0x34);
  }
  if (VGM_data_offset) {
    VGM_data_offset += 0x34;
  }
  else {
    VGM_data_offset = 0x40;
  }
  // VGM 1.51 or later
  if (0x151 <= version) {
    Sega_PCM_clock              = u32(p + 0x38);
    Sega_PCM_interface_register = u32(p + 0x3C);
  }

  if (VGM_data_offset == 0x40) {
    return;
  }
  is.read(buf + 0x40, VGM_data_offset - 0x40);

  if (0x151 <= version) {
    RF5C68_clock  = u32(p + 0x40);
    YM2203_clock  = u32(p + 0x44);
    YM2608_clock  = u32(p + 0x48);
    YM2610_clock  = u32(p + 0x4C);
    YM3812_clock  = u32(p + 0x50);
    YM3526_clock  = u32(p + 0x54);
    Y8950_clock   = u32(p + 0x58);
    YMF262_clock  = u32(p + 0x5C);
    YMF278B_clock = u32(p + 0x60);
    YMF271_clock  = u32(p + 0x64);
    YMZ280B_clock = u32(p + 0x68);
    RF5C164_clock = u32(p + 0x6C);
    PWM_clock     = u32(p + 0x70);
    AY8910_clock  = u32(p + 0x74);
    AY8910_chip_type    = u8(p + 0x78);
    AY8910_flags        = u8(p + 0x79);
    YM2203_AY8910_flags = u8(p + 0x7A);
    YM2608_AY8910_flags = u8(p + 0x7B);
  }
  // VGM 1.60 or later
  if (0x160 <= version) {
    volume_modifier = u8(p + 0x7C);
    // reserved     = u8(p + 0x7D);
    loop_base       = i8(p + 0x7E);
  }
  // VGM 1.51 or later
  if (0x151 <= version) {
    loop_modifier   = u8(p + 0x7F);
  }
  // VGM 1.61 or later
  if (0x161 <= version) {
    GameBoy_DMG_clock = u32(p + 0x80);
    NES_APU_clock     = u32(p + 0x84);
    MultiPCM_clock    = u32(p + 0x88);
    uPD7759_clock     = u32(p + 0x8C);
    OKIM6258_clock    = u32(p + 0x90);
    OKIM6258_flags    =  u8(p + 0x94);
    K054539_flags     =  u8(p + 0x95);
    C140_chip_type    =  u8(p + 0x96);
    // reserved       =  u8(p + 0x97);
    OKIM6295_clock        = u32(p + 0x98);
    K051649_K052539_clock = u32(p + 0x9C); // SCC / SCC+
    K054539_clock         = u32(p + 0xA0);
    HuC6280_clock         = u32(p + 0xA4);
    C140_clock            = u32(p + 0xA8);
    K053260_clock         = u32(p + 0xAC);
    Pokey_clock           = u32(p + 0xB0);
    QSound_clock          = u32(p + 0xB4);
  }
  // VGM 1.71 or later
  if (0x171 <= version) {
    SCSP_clock = u32(p + 0xB8);
  }
  // VGM 1.70 or later
  if (0x170 <= version) {
    extra_header_offset = u32(p + 0xBC);
  }
  // VGM 1.71 or later
  if (0x171 <= version) {
    WonderSwan_clock    = u32(p + 0xC0);
    VSU_clock           = u32(p + 0xC4);
    SAA1099_clock       = u32(p + 0xC8);
    ES5503_clock        = u32(p + 0xCC);
    ES5505_ES5506_clock = u32(p + 0xD0);
    ES5503_amount_of_output_channels        = u8(p + 0xD4);
    ES5505_ES5506_amount_of_output_channels = u8(p + 0xD5);
    C352_clock_divider                      = u8(p + 0xD6);

    X1_010_clock        = u32(p + 0xD8);
    C352_clock          = u32(p + 0xDC);
    GA20_clock          = u32(p + 0xE0);
  }
}
