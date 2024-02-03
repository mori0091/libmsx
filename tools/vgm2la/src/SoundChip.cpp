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
 * \file SoundChip.cpp
 */

#include "SoundChip.hpp"
#include <cstdint>

void SoundChip::OPLL_set(uint8_t reg, uint8_t val) {
  // printf("$%02x=%02x ", (int)reg, (int)val);
  sample_vector.push_back(reg+0xc0);
  sample_vector.push_back(val);
  soundchip_enable |= 8;
}

void SoundChip::PSG_set(uint8_t reg, uint8_t val) {
  // printf("P%02x=%02x ", (int)reg, (int)val);
  sample_vector.push_back(reg+0xb0);
  sample_vector.push_back(val);
  soundchip_enable |= 4;
  if (8 <= reg && reg <= 10 && val) {
    channel_enable |= (1 << (reg - 8));
  }
}

void SoundChip::SCC_set(uint8_t port, uint8_t reg, uint8_t val) {
  switch (port) {
    case 0:
      // SCC waveform
      if (reg < 32*4) {
        uint8_t ch = reg / 32;
        uint8_t idx = reg % 32;
        waveform[ch][idx] = (int8_t)val;
        if (!(waveform_updated & (1 << ch))) {
          waveform_updated |= (1 << ch);
          sample_vector.push_back(0xfa);
          sample_vector.push_back(ch);
        }
        soundchip_enable |= 1;
        // printf("S%02x=%02x ", (int)reg, (int)val);
      }
      break;
    case 1:
      // SCC/SCC+ FDR
      if (reg < 2*5) {
        sample_vector.push_back(reg+0xa0);
        sample_vector.push_back(val);
        soundchip_enable |= 1;
        // printf("S%02x=%02x ", (int)0xa0+reg, (int)val);
      }
      break;
    case 2:
      // SCC/SCC+ Volume
      if (reg < 5) {
        sample_vector.push_back(reg+0xaa);
        sample_vector.push_back(val);
        soundchip_enable |= 1;
        if (val) {
          channel_enable |= (8 << reg);
        }
        // printf("S%02x=%02x ", (int)0xaa+reg, (int)val);
      }
      break;
    case 3:
      // SCC/SCC+ channel mask
      if (reg < 1) {
        sample_vector.push_back(0xaf);
        sample_vector.push_back(val);
        soundchip_enable |= 1;
        // printf("Saf=%02x ", (int)val);
      }
      break;
    case 4:
      // SCC+ waveform
      if (reg < 32*5) {
        uint8_t ch = reg / 32;
        uint8_t idx = reg % 32;
        waveform[ch][idx] = (int8_t)val;
        if (!(waveform_updated & (1 << ch))) {
          waveform_updated |= (1 << ch);
          sample_vector.push_back(0xfa);
          sample_vector.push_back(ch);
        }
        soundchip_enable |= 2;
        // printf("S%02x=%02x ", (int)reg, (int)val);
      }
      break;
    case 5:
      // SCC/SCC+ deformation / test register
      if (reg < 1) {
        // not supported
        soundchip_enable |= 1;
        // printf("!Sc0=%02x ", (int)val);
      }
      break;
    default:
      break;
  }
}

static uint8_t find_waveform(const SoundChip::SCC_Waveform & wave, std::vector<SoundChip::SCC_Waveform> & wavedb) {
  for (size_t idx = 0; idx < wavedb.size(); ++idx) {
    if (wavedb[idx] == wave) {
      return idx;
    }
  }
  wavedb.push_back(wave);
  assert(wavedb.size() < 255);
  return wavedb.size() - 1;
}

SoundChip::Sample SoundChip::sample(std::vector<SCC_Waveform> & wavedb) {
  // printf("\n");
  if (waveform_updated) {
    for (size_t i = 0; i < sample_vector.size(); i += 2) {
      if (sample_vector[i] == 0xfa) {
        uint8_t ch = sample_vector[i+1];
        uint8_t idx = find_waveform(waveform[ch], wavedb);
        sample_vector[i] += ch; // 0xfa + ch
        sample_vector[i+1] = idx;
      }
    }
  }
  waveform_updated = 0;
  SoundChip::Sample ret{sample_vector};
  sample_vector.clear();
  return ret;
}
