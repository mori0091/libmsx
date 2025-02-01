// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file snd_get_bgm_frequency.c
 *
 */

#include "../include/snddrv.h"
#include "./snd_ctx.h"

uint8_t snd_get_bgm_frequency(void) {
  uint8_t freq;
  DI();
  freq = snd_bgm.song_freq;
  EI();
  return freq;
}
