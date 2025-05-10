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
 * \file audio_play.c
 */

#include "audio.h"
#include "audio__internal.h"

#include <audio_buf.h>

void audio_decode(void) {
  if (audio_.bgm.is_playing) {
    audio_.bgm.counter += audio_.bgm.rate;
    while (audio_.vsync_freq <= audio_.bgm.counter) {
      audio_.bgm.counter -= audio_.vsync_freq;
      if (!audio_.bgm.decoder.decode_update()) {
        audio_.bgm.is_playing = false;
        audio_.bgm.counter = 0;
        break;
      }
    }
    audio_.bgm.decoder.decode_final();
  }

  if (audio_.sfx.is_playing) {
    audio_.sfx.counter += audio_.sfx.rate;
    while (audio_.vsync_freq <= audio_.sfx.counter) {
      audio_.sfx.counter -= audio_.vsync_freq;
      if (!audio_.sfx.decoder.decode_update()) {
        audio_.sfx.is_playing = false;
        audio_.sfx.counter = 0;
        audio_buf_restore();
        break;
      }
    }
    audio_.sfx.decoder.decode_final();
  }
}

void audio_play(void) {
  if (audio_.paused) return;
  bool b = audio_is_playing();
  audio_buf_play();
  audio_decode();
  if (b && !audio_is_playing()) {
    audio_buf_init();
  }
}
