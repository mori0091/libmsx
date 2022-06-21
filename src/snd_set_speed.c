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
 * \file snd_set_speed.c
 *
 */

#include "../include/snddrv.h"
#include "./snd_ctx.h"

extern uint8_t snd_speed_multiplier4x;

void snd_set_speed(uint8_t multiplier4x) {
  snd_speed_multiplier4x = multiplier4x;
  snd_set_player_frequency((int)multiplier4x * snd_bgm.song_freq / 4);
}
