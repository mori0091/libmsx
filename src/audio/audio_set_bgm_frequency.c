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
 * \file audio_set_player_frequency.c
 */

#include "audio__internal.h"

void audio_set_bgm_frequency(uint8_t freq) {
  __asm__("di");
  audio_.bgm.rate = freq;
  __asm__("ei");
}
