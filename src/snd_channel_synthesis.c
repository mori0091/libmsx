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
 * \file snd_channel_synthesis.c
 *
 */

#include "./snd_channel.h"
#include "./snd_osc_period.h"

#include "../include/ay_3_8910.h"
#include <stdint.h>

#define PSG(reg)                     (ay_3_8910_buffer[(reg)])

static void snd_channel_set_noise(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_tone_flag(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_volume(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_modulation(uint8_t ch, struct snd_channel * pch);

void snd_channel_synthesis(uint8_t ch, struct snd_channel * pch) {
  if (pch->pitch < 0 || 4 < pch->i.modulation) {
    PSG(ch+8) = 0;
  }
  else {
    // noise -------------------------------------------
    snd_channel_set_noise(ch, pch);
    // tone on/off -------------------------------------
    snd_channel_set_tone_flag(ch, pch);
    // volume or enable hardware envelope --------------
    snd_channel_set_volume(ch, pch);
    // modulation --------------------------------------
    snd_channel_set_modulation(ch, pch);
  }
}

static void snd_channel_set_noise(uint8_t ch, struct snd_channel * pch) {
  if (pch->i.noise_fdr) {
    PSG(6) = pch->i.noise_fdr;
    PSG(7) &= ~(8 << ch);
  }
}
static void snd_channel_set_tone_flag(uint8_t ch, struct snd_channel * pch) {
  if (!pch->i.tone_on || pch->i.modulation == 1) {
    PSG(7) |= (1 << ch);
  }
}
static void snd_channel_set_volume(uint8_t ch, struct snd_channel * pch) {
  if (!pch->i.modulation) {
    int8_t volume = pch->volume + pch->i.volume - 15; // (<= 15)
    if (volume < 0) {
      volume = 0;
    }
    // else if (15 < volume) {
    //   volume = 15;
    // }
    PSG(ch+8) = volume;
  }
  else {
    PSG(ch+8) = 16;
  }
}

static int16_t snd_channel_calc_pitch(struct snd_channel * pch);
static uint16_t snd_channel_calc_sw_period(int16_t pitch, struct snd_channel * pch);
static uint16_t snd_channel_calc_hw_period(int16_t pitch, struct snd_channel * pch);
static void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period);
static void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period);

static void snd_channel_set_modulation_sw_only(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = snd_channel_calc_pitch(pch);
  const uint16_t sw_period = snd_channel_calc_sw_period(pitch, pch);
  const int16_t period_offset = pch->p.value;
  snd_channel_set_sw_period(ch, sw_period + period_offset);
}

static void snd_channel_set_modulation_hw_only(uint8_t ch, struct snd_channel * pch) {
  (void)ch;
  const int16_t pitch = snd_channel_calc_pitch(pch);
  const uint16_t hw_period = snd_channel_calc_hw_period(pitch, pch);
  const int16_t period_offset = pch->p.value;
  snd_channel_set_hw_period(pch, hw_period + period_offset);
}

static void snd_channel_set_modulation_sw_to_hw(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = snd_channel_calc_pitch(pch);
  uint16_t sw_period = snd_channel_calc_sw_period(pitch, pch);
  uint16_t hw_period = pch->i.hw_period;
  if (!hw_period) {
    hw_period = (sw_period >> pch->i.ratio) + pch->i.hw_period_delta;
  }
  const int16_t period_offset = pch->p.value;
  sw_period += period_offset;
  hw_period += period_offset;
  snd_channel_set_sw_period(ch, sw_period);
  snd_channel_set_hw_period(pch, hw_period);
}

static void snd_channel_set_modulation_hw_to_sw(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = snd_channel_calc_pitch(pch);
  uint16_t hw_period = snd_channel_calc_hw_period(pitch, pch);
  uint16_t sw_period = pch->i.sw_period;
  if (!sw_period) {
    sw_period = (hw_period << pch->i.ratio) + pch->i.sw_period_delta;
  }
  const int16_t period_offset = pch->p.value;
  sw_period += period_offset;
  hw_period += period_offset;
  snd_channel_set_sw_period(ch, sw_period);
  snd_channel_set_hw_period(pch, hw_period);
}

static void snd_channel_set_modulation_hw_and_sw(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = snd_channel_calc_pitch(pch);
  const int16_t period_offset = pch->p.value;
  const uint16_t sw_period = snd_channel_calc_sw_period(pitch, pch) + period_offset;
  snd_channel_set_sw_period(ch, sw_period);
  const uint16_t hw_period = snd_channel_calc_hw_period(pitch, pch) + period_offset;
  snd_channel_set_hw_period(pch, hw_period);
}

typedef void (* snd_modulation_func)(uint8_t, struct snd_channel *);
static const snd_modulation_func modulations[] = {
  [0] = snd_channel_set_modulation_sw_only,
  [1] = snd_channel_set_modulation_hw_only,
  [2] = snd_channel_set_modulation_sw_to_hw,
  [3] = snd_channel_set_modulation_hw_to_sw,
  [4] = snd_channel_set_modulation_hw_and_sw,
};

static void snd_channel_set_modulation(uint8_t ch, struct snd_channel * pch) {
  modulations[pch->i.modulation](ch, pch);
}

static int16_t snd_channel_calc_pitch(struct snd_channel * pch) {
  return pch->pitch + 256 * pch->arp + pch->a.value;
}

static uint16_t snd_channel_calc_sw_period(int16_t pitch, struct snd_channel * pch) {
  if (!pch->i.sw_period) {
    return
      snd_osc_period(pitch + pch->i.sw_pitch)
      + pch->period_offset
      + pch->i.sw_period_delta;
  }
  else {
    return
      pch->i.sw_period
      + pch->period_offset;
  }
}
static uint16_t snd_channel_calc_hw_period(int16_t pitch, struct snd_channel * pch) {
  if (!pch->i.hw_period) {
    return
      snd_osc_period(pitch + pch->i.hw_pitch)
      + pch->period_offset      // !?
      + pch->i.hw_period_delta;
  }
  else {
    return
      pch->i.hw_period
      + pch->period_offset;     // !?
  }
}
static void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period) {
  ch <<= 1;
  PSG(ch) = (sw_period     ) & 0xff;
  ch++;
  PSG(ch) = (sw_period >> 8) & 0x0f;
}
static void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period) {
  PSG(11) = (hw_period     ) & 0xff;
  PSG(12) = (hw_period >> 8) & 0xff;
  if (pch->i.retrig) {
    // R13 = 8:Saw, 10:Triangle, 12:Inv-Saw, 14:Inv-Triangle
    PSG(13) = pch->i.waveform;
  }
}
