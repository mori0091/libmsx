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
 * \file SoundChip.hpp
 */

#ifndef SOUNDCHIP_HPP_
#define SOUNDCHIP_HPP_

#include <array>
#include <cassert>
#include <cstdint>
#include <ios>
#include <ostream>
#include <vector>

struct SoundChip {

  using SCC_Waveform = std::array<int8_t, 32>;
  using Sample = std::vector<uint8_t>;

  std::array<bool, 256> updated;
  std::array<uint8_t, 256> latest_value;
  uint8_t soundchip_enable;
  uint8_t channel_enable;       // Using channels of SCC/SCC+ and PSG

  uint8_t waveform_updated;
  SCC_Waveform waveform[5];
  Sample sample_vector;

  SoundChip() : updated(), latest_value(), soundchip_enable(0), channel_enable(0), waveform_updated(0), waveform(), sample_vector() {}

  /**
   * Set a value to OPLL register.
   *
   * \param reg   Register number. [0x00..0x07, 0x0e, 0x10..0x18, 0x20..0x28, 0x30..0x38]
   * \param val   Value to be set to the register.
   */
  void OPLL_set(uint8_t reg, uint8_t val);

  /**
   * Set a value to PSG register.
   *
   * \param reg   Register number. [0x00..0x0d]
   * \param val   Value to be set to the register.
   */
  void PSG_set(uint8_t reg, uint8_t val);

  /**
   * Set a value to SCC/SCC+ register.
   *
   * \param port  Specify a set of registers:
   *              - 0: waveform for SCC (reg < 32 byte * 4 channels)
   *              - 1: frequency division ratio (reg < 2 byte * 5 channels)
   *              - 2: volume (reg < 1 byte * 5 channels)
   *              - 3: channel mask (reg < 1)
   *              - 4: waveform for SCC+ (reg < 32 byte * 5 channels)
   *              - 5: deformation (reg < 1)
   * \param reg   Offset from the base address of a set of registers.
   * \param val   Value to be set to the register.
   */
  void SCC_set(uint8_t port, uint8_t reg, uint8_t val);

  /**
   * Sample and return the current value of registers.
   * \param wavedb[inout] vector of waveforms.
   * \return the current value of registers.
   */
  Sample sample(std::vector<SCC_Waveform> & wavedb);

  void put(uint8_t cmd, uint8_t val) {
    if (cmd == 0xbd || cmd == 0xfa || !updated[cmd] || latest_value[cmd] != val) {
      sample_vector.push_back(cmd);
      sample_vector.push_back(val);
      updated[cmd] = true;
      latest_value[cmd] = val;
    }
  }
};

#endif // SOUNDCHIP_HPP_
