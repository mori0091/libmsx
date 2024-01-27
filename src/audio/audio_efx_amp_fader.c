// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file audio_efx_amp_fader.c
 */

#include <audio_efx_amp.h>

static uint8_t fade_period;
static uint8_t fade_timer;
static int8_t fade;

void audio_efx_amp__fader(void) {
  if (!fade) return;
  if (fade_timer) {
    fade_timer--;
    return;
  }
  fade_timer = fade_period;
  uint8_t vol = audio_efx_amp_get_volume();
  if (0 < fade && 15 <= vol) {
    fade = 0;
  }
  else if (fade < 0 && !vol) {
    fade = 0;
  }
  else {
    audio_efx_amp_set_volume(vol + fade);
  }
}

void audio_efx_amp_fadeout(uint8_t ticks) {
  fade_period = fade_timer = ticks;
  fade = -1;
}

void audio_efx_amp_fadein(uint8_t ticks) {
  fade_period = fade_timer = ticks;
  fade = +1;
}
