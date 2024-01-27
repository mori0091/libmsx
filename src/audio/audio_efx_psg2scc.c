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
 * \file audio_efx_psg2scc.c
 */

#include <audio_efx_psg2scc.h>

#include <audio_buf.h>
#include <scc_wav.h>

#include <string.h>

void audio_efx_psg2scc(void) {
  if (!audio_buf_cache[0xfa]) {
    memcpy(&audio_buf_cache[0x00], SCC_WAVEFORM_PULSE_1_32, 32);
    memcpy(&audio_buf_cache[0x20], SCC_WAVEFORM_PULSE_1_32, 32);
    memcpy(&audio_buf_cache[0x40], SCC_WAVEFORM_PULSE_1_32, 32);
    audio_buf_put(0, 0xfa, 0);
    audio_buf_put(0, 0xfa, 1);
    audio_buf_put(0, 0xfa, 2);
  }

  audio_buf_put(0, 0xaf, 0x07);

  audio_buf_put(0, 0xa0, audio_buf_cache[0xb0]); // ch.1 FDR LO
  audio_buf_put(0, 0xa1, audio_buf_cache[0xb1]); // ch.1 FDR HI
  audio_buf_put(0, 0xa2, audio_buf_cache[0xb2]); // ch.2 FDR LO
  audio_buf_put(0, 0xa3, audio_buf_cache[0xb3]); // ch.2 FDR HI
  audio_buf_put(0, 0xa4, audio_buf_cache[0xb4]); // ch.3 FDR LO
  audio_buf_put(0, 0xa5, audio_buf_cache[0xb5]); // ch.3 FDR HI

  audio_buf_put(0, 0xaa, audio_buf_cache[0xb8] < 16 ? audio_buf_cache[0xb8] : 15); // ch.1 Volume
  audio_buf_put(0, 0xab, audio_buf_cache[0xb9] < 16 ? audio_buf_cache[0xb9] : 15); // ch.2 Volume
  audio_buf_put(0, 0xac, audio_buf_cache[0xba] < 16 ? audio_buf_cache[0xba] : 15); // ch.3 Volume
}
