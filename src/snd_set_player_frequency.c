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
 * \file snd_set_player_frequency.c
 *
 */

#include "../include/snddrv.h"
#include "./snd_ctx.h"
#include "audio.h"

void snd_set_player_frequency(uint8_t freq) {
  audio_set_bgm_frequency(freq);
}
