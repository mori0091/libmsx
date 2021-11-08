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

// --- Background music ----

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
  },
};

// --- Sound effect ----

// sound data stream for PSG channel A
const uint8_t se_channel_1[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x00, 0xd6, /* T180 O5 C32 S0 M4096 */
  0x60, 0x03, 0x90, 0x00, 0xca,             /* T180    C+32 */
  0x60, 0x05, 0x90, 0x00, 0xbe,             /* T180    D16 */
  0xff,
};

// sound data stream for PSG channel B
const uint8_t se_channel_2[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x01, 0xac, /* T180 O4 C32 S0 M4096 */
  0x60, 0x03, 0x90, 0x01, 0x94,             /* T180    C+32 */
  0x60, 0x05, 0x90, 0x01, 0x7d,             /* T180    D16 */
  0xff,
};

// sound data stream for PSG channel C
const uint8_t se_channel_3[] = {
  0xff,
};

// the sound clip structure that holds pointers to the sound data streams.
const struct sound_clip se = {
  .priority = 0,
  .streams = {
    [0] = se_channel_1,
    [1] = se_channel_2,
    [2] = se_channel_3,
  },
};

_Noreturn
static void infinite_loop(void) {
  uint16_t t = 0;
  for (;;) {
    t++;
    await_vsync();
    if (t % 170 == 0) {
      sound_effect(&se);        // play sound effects
    }
  }
}

void main(void) {
  // Initialize the sound driver.
  // This must be called once at least, before the sound_player() is called.
  sound_init();

  // Register the sound driver as VSYNC handler.
  set_vsync_handler(sound_player);

  sound_set_repeat(true);       // Turn on the auto-repeat of BGM.
  sound_set_bgm(&bgm);          // Register the BGM to be played.
  sound_start();                // Start the BGM.

  infinite_loop();
}
