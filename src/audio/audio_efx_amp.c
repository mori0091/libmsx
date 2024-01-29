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
 * \file audio_efx_amp.c
 */

#include <audio_efx_amp.h>
#include <audio_buf.h>

#include <stdbool.h>

static int8_t main_attenuation; // -15..0
static uint8_t fade_period;
static uint8_t fade_timer;
static int8_t fade;             // -1, 0, or +1

static void set_volume(uint8_t volume) {
  if (volume < 16) {
    main_attenuation = volume - 15;
  }
}

static uint8_t get_volume(void) {
  return main_attenuation + 15;
}

static bool fader(void) {
  if (!fade) return false;
  if (fade_timer) {
    fade_timer--;
    return true;
  }
  fade_timer = fade_period;
  uint8_t vol = get_volume();
  if (0 < fade && 15 <= vol) {
    fade = 0;
    return false;
  }
  else if (fade < 0 && !vol) {
    fade = 0;
    return false;
  }
  else {
    set_volume(vol + fade);
    return true;
  }
}

static uint8_t volume(uint8_t volume) {
  if (16 <= volume) return volume; // PSG H/W envelope
  const int8_t x = volume + main_attenuation;
  if (x < 0) {
    return 0;
  }
  return x;
}

static void attenuate_opll_6(void) {
  // 6 ch + RHYTHM
  uint8_t reg = 0x30;
  for ( ; reg < 0x37; reg++) {
    const uint8_t val0 = audio_buf_cache[(uint8_t)(0xc0 | reg)];
    int8_t v = (val0 & 0x0f) - main_attenuation;
    if (16 <= v) v = 15;
    const uint8_t val = (val0 & 0xf0) | v;
    audio_buf_put(255, (uint8_t)(0xc0 | reg), val);
  }
  for ( ; reg < 0x39; reg++) {
    const uint8_t val0_ = audio_buf_cache[(uint8_t)(0xc0 | reg)];
    int8_t vl = ((val0_ >> 0) & 0x0f) - main_attenuation;
    if (16 <= vl) vl = 15;
    int8_t vh = ((val0_ >> 4) & 0x0f) - main_attenuation;
    if (16 <= vh) vh = 15;
    const uint8_t val_ = (vh << 4) | vl;
    audio_buf_put(255, (uint8_t)(0xc0 | reg), val_);
  }
}

static void attenuate_opll_9(void) {
  // 9 ch
  for (uint8_t reg = 0x30; reg < 0x39; reg++) {
    const uint8_t val0 = audio_buf_cache[(uint8_t)(0xc0 | reg)];
    int8_t v = (val0 & 0x0f) - main_attenuation;
    if (16 <= v) v = 15;
    const uint8_t val = (val0 & 0xf0) | v;
    audio_buf_put(255, (uint8_t)(0xc0 | reg), val);
  }
}

void audio_efx_amp(void) {
  bool b = fader();
  if (!main_attenuation) return;
  if (!b && main_attenuation <= -15) {
    audio_buf_clear();
  }
  else {
    audio_buf_put(255, 0xb8, volume(audio_buf_cache[0xb8]));
    audio_buf_put(255, 0xb9, volume(audio_buf_cache[0xb9]));
    audio_buf_put(255, 0xba, volume(audio_buf_cache[0xba]));
    audio_buf_put(255, 0xaa, volume(audio_buf_cache[0xaa]));
    audio_buf_put(255, 0xab, volume(audio_buf_cache[0xab]));
    audio_buf_put(255, 0xac, volume(audio_buf_cache[0xac]));
    audio_buf_put(255, 0xad, volume(audio_buf_cache[0xad]));
    audio_buf_put(255, 0xae, volume(audio_buf_cache[0xae]));
    if (audio_buf_cache[0xce] & 0x20) {
      attenuate_opll_6();
    }
    else {
      attenuate_opll_9();
    }
  }
}

void audio_efx_amp_set_volume(uint8_t volume) {
  __asm__("di");
  set_volume(volume);
  __asm__("ei");
}

uint8_t audio_efx_amp_get_volume(void) {
  uint8_t ret;
  __asm__("di");
  ret = get_volume();
  __asm__("ei");
  return ret;
}

void audio_efx_amp_fadeout(uint8_t ticks) {
  __asm__("di");
  fade_period = fade_timer = ticks;
  fade = -1;
  __asm__("ei");
}

void audio_efx_amp_fadein(uint8_t ticks) {
  __asm__("di");
  fade_period = fade_timer = ticks;
  fade = +1;
  __asm__("ei");
}

int8_t audio_efx_amp_get_fade(void) {
  int8_t ret;
  __asm__("di");
  ret = fade;
  __asm__("ei");
  return fade;
}
