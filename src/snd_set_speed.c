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
 * \file snd_set_speed.c
 *
 */

#include "../include/snddrv.h"
#include "./snd_ctx.h"

extern uint8_t snd_speed_multiplier;

extern void snd__set_bgm_freq(uint8_t freq);

void snd__set_speed(uint8_t multiplier) {
  snd_speed_multiplier = multiplier;
  snd__set_bgm_freq((int)snd_bgm.song_freq * multiplier / SND_SPEED_1X);
}

void snd_set_speed(uint8_t multiplier) {
  DI();
  snd__set_speed(multiplier);
  EI();
}
