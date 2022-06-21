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
 * \file snd_set_player_frequency.c
 *
 */

#include "../include/snddrv.h"
#include "./snd_ctx.h"

#define DI() __asm__("di")
#define EI() __asm__("ei")

inline void snd__set_bgm_freq(uint8_t freq) {
  if (freq <= 0) {
    freq = 1;
  }
  if (240 <= freq) {
    freq = 240;
  }
  snd_bgm.play_freq = freq;
}

void snd_set_player_frequency(uint8_t freq) {
  DI();
  snd__set_bgm_freq(freq);
  EI();
}
