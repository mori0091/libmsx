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
 * \file AudioSample.hpp
 */

#ifndef AUDIOSAMPLE_H_
#define AUDIOSAMPLE_H_

#include "SoundChip.hpp"
#include "VGMHeader.hpp"

struct AudioSample {
  double rate;
  uint8_t soundchip_enable;
  uint8_t channel_enable;

  std::vector<SoundChip::SCC_Waveform> scc_wavtbl;
  std::vector<SoundChip::Sample> samples;

  void sample(SoundChip & x) {
    soundchip_enable |= x.soundchip_enable;
    channel_enable |= x.channel_enable;
    SoundChip::Sample s = x.sample(scc_wavtbl);
    samples.push_back(s);
  }

  void dump(const VGMHeader & vgm_header, std::ostream & os);
};

#endif // AUDIOSAMPLE_H_
