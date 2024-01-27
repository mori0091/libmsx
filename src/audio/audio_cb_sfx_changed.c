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
 * \file audio_cb_sfx_changed.c
 */

#include "audio__internal.h"

void audio_cb_sfx_changed(uint8_t song_rate, const AudioDecoder * decoder) {
  __asm__("di");
  audio_.sfx.is_playing = false;
  audio_.sfx.decoder.decode_update = audio__null_decode_update;
  audio_.sfx.decoder.decode_final = audio__null_decode_final;
  audio_.sfx.decoder.set_repeat = audio__null_set_repeat;
  if (decoder) {
    if (decoder->decode_update) {
      audio_.sfx.decoder.decode_update = decoder->decode_update;
    }
    if (decoder->decode_final) {
      audio_.sfx.decoder.decode_final = decoder->decode_final;
    }
    // if (decoder->set_repeat) {
    //   audio_.sfx.decoder.set_repeat = decoder->set_repeat;
    // }
  }
  if (!(audio_.sfx.rate = song_rate)) {
    audio_.sfx.rate = audio_.vsync_freq;
  }
  audio_.sfx.counter = 0;
  audio_.sfx.is_playing = true;
  // audio_.sfx.decoder.set_repeat(audio_.repeat);
}
