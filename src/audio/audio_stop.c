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
 * \file audio_stop.c
 */

#include "audio__internal.h"

void audio_stop(void) {
  audio_pause();
  __asm__("di");
  audio_.bgm.is_playing = false;
  audio_.sfx.is_playing = false;
  __asm__("ei");
}
