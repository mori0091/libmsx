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
 * \file audio_play.c
 */

#include "audio__internal.h"

#include <ay_3_8910.h>
#include <scc_buf.h>
#include <opll_buf.h>

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
        break;
      }
    }
    audio_.sfx.decoder.decode_final();
  }
}

void audio_play(void) {
  if (audio_.paused) return;
  ay_3_8910_play();
  SCC_play(&audio_.scc);
  OPLL_play(&audio_.opll);
  if (audio_is_playing()) {
    audio_decode();
    if (!audio_is_playing()) {
      ay_3_8910_stop();
      SCC_stop(&audio_.scc);
      OPLL_stop(&audio_.opll);
    }
  }
}
