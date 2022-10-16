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
 * \file snd_channel_synthesis.c
 *
 */

#include "./snd_channel.h"
#include "./snd_osc_period.h"

#include "../include/ay_3_8910.h"

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
    int8_t volume = pch->volume + pch->i.volume - 15;
    if (volume < 0) {
      volume = 0;
    }
    else if (15 < volume) {
      volume = 15;
    }
    PSG(ch+8) = volume;
  }
  else {
    PSG(ch+8) = 16;
  }
}

static int16_t snd_channel_calc_pitch(struct snd_channel * pch);
static uint16_t snd_channel_calc_sw_period(int16_t pitch, struct snd_channel * pch);
static uint16_t snd_channel_calc_hw_period(int16_t pitch, struct snd_channel * pch);
inline void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period);
inline void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period);

static void snd_channel_set_modulation(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = snd_channel_calc_pitch(pch);
  uint16_t sw_period = 0;
  uint16_t hw_period = 0;
  switch (pch->i.modulation) {
    case 0:
      // ---- SW only ------------------------------------
      sw_period = snd_channel_calc_sw_period(pitch, pch);
      break;
    case 1:
      // ---- HW only ------------------------------------
      hw_period = snd_channel_calc_hw_period(pitch, pch);
      break;
    case 2:
      // ---- SW -> HW -----------------------------------
      sw_period = snd_channel_calc_sw_period(pitch, pch);
      hw_period = pch->i.hw_period;
      if (!hw_period) {
        hw_period = (sw_period >> pch->i.ratio) + pch->i.hw_period_delta;
      }
      break;
    case 3:
      // ----- HW -> SW -----------------------------------
      hw_period = snd_channel_calc_hw_period(pitch, pch);
      sw_period = pch->i.sw_period;
      if (!sw_period) {
        sw_period = (hw_period << pch->i.ratio) + pch->i.sw_period_delta;
      }
      break;
    case 4:
      // ----- SW + HW ------------------------------------
      sw_period = snd_channel_calc_sw_period(pitch, pch);
      hw_period = snd_channel_calc_hw_period(pitch, pch);
      break;
  }
  // ---- square wave ---------------------------------
  snd_channel_set_sw_period(ch, sw_period);
  // ---- saw / triangle wave -------------------------
  snd_channel_set_hw_period(pch, hw_period);
}

static int16_t snd_channel_calc_pitch(struct snd_channel * pch) {
  return pch->pitch + 256 * pch->arp + pch->p.pitch;
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
      (snd_osc_period(pitch + pch->i.hw_pitch) >> pch->i.ratio)
      + pch->period_offset      // !?
      + pch->i.hw_period_delta;
  }
  else {
    return
      pch->i.hw_period
      + pch->period_offset;     // !?
  }
}
inline void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period) {
  if (sw_period) {
    ch <<= 1;
    PSG(ch) = (sw_period     ) & 0xff;
    ch++;
    PSG(ch) = (sw_period >> 8) & 0x0f;
  }
}
inline void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period) {
  if (hw_period) {
    PSG(11) = (hw_period     ) & 0xff;
    PSG(12) = (hw_period >> 8) & 0xff;
    if (pch->i.retrig) {
      // R13 = 8:Saw, 10:Triangle, 12:Inv-Saw, 14:Inv-Triangle
      PSG(13) = pch->i.waveform;
    }
  }
}
