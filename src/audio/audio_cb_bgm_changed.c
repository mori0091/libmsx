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
 * \file audio_cb_bgm_changed.c
 */

#include "audio__internal.h"

#include <ay_3_8910.h>
#include <scc_buf.h>
#include <opll_buf.h>

void audio_cb_bgm_changed(uint8_t song_rate, const AudioDecoder * decoder) {
  __asm__("di");
  audio_.bgm.is_playing = false;
  audio_.bgm.decoder.decode_update = audio__null_decode_update;
  audio_.bgm.decoder.decode_final = audio__null_decode_final;
  audio_.bgm.decoder.set_repeat = audio__null_set_repeat;
  if (decoder) {
    if (decoder->decode_update) {
      audio_.bgm.decoder.decode_update = decoder->decode_update;
    }
    if (decoder->decode_final) {
      audio_.bgm.decoder.decode_final = decoder->decode_final;
    }
    if (decoder->set_repeat) {
      audio_.bgm.decoder.set_repeat = decoder->set_repeat;
    }
  }
  if (!(audio_.bgm.rate = song_rate)) {
    audio_.bgm.rate = audio_.vsync_freq;
  }
  audio_.bgm.counter = 0;
  audio_.bgm.is_playing = true;
  audio_.bgm.decoder.set_repeat(audio_.repeat);

  ay_3_8910_init();
  SCC_init();
  OPLL_init();
}
