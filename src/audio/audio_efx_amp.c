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
 * \file audio_efx_amp.c
 */

#include <audio_efx_amp.h>

#include <ay_3_8910.h>
#include <scc_buf.h>
#include <opll_buf.h>

static int8_t main_attenuation;

static uint8_t volume(uint8_t volume) {
  if (16 <= volume) return volume;
  const int8_t x = volume + main_attenuation;
  if (x < 0) {
    return 0;
  }
  return x;
}

void audio_efx_amp_set_volume(uint8_t volume) {
  if (volume < 16) {
    main_attenuation = volume - 15;
  }
}

uint8_t audio_efx_amp_get_volume(void) {
  return main_attenuation + 15;
}

extern void audio_efx_amp__fader(void);

void audio_efx_amp(void) {
  audio_efx_amp__fader();
  if (!main_attenuation) return;
  if (main_attenuation <= -15) {
    // mute all channels.
    ay_3_8910_buffer[8] = 0;
    ay_3_8910_buffer[9] = 0;
    ay_3_8910_buffer[10] = 0;
    scc_buffer.volume[0] = 0;
    scc_buffer.volume[1] = 0;
    scc_buffer.volume[2] = 0;
    scc_buffer.volume[3] = 0;
    scc_buffer.volume[4] = 0;
    for (uint8_t reg = 0x30; reg < 0x3a; reg++) {
      opll_buffer[reg] |= 0x0f;
    }
  }
  else {
    // attenuate volume level of each channels.
    ay_3_8910_buffer[8] = volume(ay_3_8910_buffer[8]);
    ay_3_8910_buffer[9] = volume(ay_3_8910_buffer[9]);
    ay_3_8910_buffer[10] = volume(ay_3_8910_buffer[10]);
    scc_buffer.volume[0] = volume(scc_buffer.volume[0]);
    scc_buffer.volume[1] = volume(scc_buffer.volume[1]);
    scc_buffer.volume[2] = volume(scc_buffer.volume[2]);
    scc_buffer.volume[3] = volume(scc_buffer.volume[3]);
    scc_buffer.volume[4] = volume(scc_buffer.volume[4]);
    for (uint8_t reg = 0x30; reg < 0x3a; reg++) {
      uint8_t val = opll_buffer[reg];
      int8_t v = (val & 0x0f) - main_attenuation;
      if (16 <= v) v = 15;
      opll_buffer[reg] = (val & 0xf0) | v;
    }
  }
}
