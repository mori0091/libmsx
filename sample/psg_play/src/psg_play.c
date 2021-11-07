// -*- coding: utf-8-unix -*-
/**
 * \file psg_play.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include <msx.h>

// see also https://mori0091.github.io/libmsx/structsound__clip.html
//      and https://mori0091.github.io/libmsx/sound_8h.html

// sound data stream for PSG channel A
const uint8_t channel_1[] ={
  0x60, 0x1e, 0x88, 0x01, 0xac, // T120L4 V8 C
  0x40, 0x1e, 0x01, 0x7d,       // T120L4    D
  0x40, 0x1e, 0x01, 0x53,       // T120L4    E
  0x20, 0x1e, 0x10,             // T120L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel B
const uint8_t channel_2[] = {
  0x60, 0x1e, 0x88, 0x00, 0xd6, // T120L4 V8 O5C
  0x40, 0x1e, 0x00, 0xbe,       // T120L4    O5D
  0x40, 0x1e, 0x00, 0xaa,       // T120L4    O5E
  0x20, 0x1e, 0x10,             // T120L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel C
const uint8_t channel_3[] = {
  0x60, 0x1e, 0x88, 0x02, 0x3b, // T120L4 V8 O3G
  0x40, 0x1e, 0x01, 0xfd,       // T120L4    O3A
  0x40, 0x1e, 0x01, 0xc5,       // T120L4    O3B
  0x20, 0x1e, 0x10,             // T120L4    R
  0xff,                         // end of stream
};

// the sound clip structure that holds pointers to the sound data streams.
const struct sound_clip bgm = {
  .streams = {
    [0] = channel_1,
    [1] = channel_2,
    [2] = channel_3,
  }
};

_Noreturn
static void infinite_loop(void) {
  for (;;) {
    await_vsync();
  }
}

void main(void) {
  set_vsync_handler(sound_player); // Register the sound driver as VSYNC handler

  sound_set_repeat(true);          // Turn on the auto-repeat of BGM.
  sound_set_bgm(&bgm);             // Register the BGM to be played.
  sound_start();                   // Start the BGM.

  infinite_loop();
}
