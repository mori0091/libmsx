// -*- coding: utf-8-unix -*-
/**
 * \file psg_play.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include <msx.h>

// see also
// - https://mori0091.github.io/libmsx/sound_8h.html
// - https://mori0091.github.io/libmsx/structsound__clip.html
// - https://mori0091.github.io/libmsx/structsound__fragment.html

#define ARRAY_SIZEOF(a)    (sizeof(a) / sizeof(a[0]))

// ---- Background music ----
// ---- (A part) ----

// sound data stream for PSG channel A
const uint8_t bgm_part_A_ch_1[] ={
  0x60, 0x1e, 0x88, 0x01, 0xac, // T120 L4 V8 C
  0x40, 0x1e, 0x01, 0x7d,       // T120 L4    D
  0x40, 0x1e, 0x01, 0x53,       // T120 L4    E
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel B
const uint8_t bgm_part_A_ch_2[] = {
  0x60, 0x1e, 0x88, 0x00, 0xd6, // T120 L4 V8 O5C
  0x40, 0x1e, 0x00, 0xbe,       // T120 L4    O5D
  0x40, 0x1e, 0x00, 0xaa,       // T120 L4    O5E
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel C
const uint8_t bgm_part_A_ch_3[] = {
  0x60, 0x1e, 0x88, 0x02, 0x3b, // T120 L4 V8 O3G
  0x40, 0x1e, 0x01, 0xfd,       // T120 L4    O3A
  0x40, 0x1e, 0x01, 0xc5,       // T120 L4    O3B
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// the sound fragment structure that holds pointers to the sound data streams.
const struct sound_fragment bgm_A_part = {
  .streams = {
    [0] = bgm_part_A_ch_1,
    [1] = bgm_part_A_ch_2,
    [2] = bgm_part_A_ch_3,
  },
};

// ---- (B part) ----

// sound data stream for PSG channel A
const uint8_t bgm_part_B_ch_1[] ={
  0x60, 0x1e, 0x88, 0x01, 0xac, // T120 L4 V8 C
  0x40, 0x1e, 0x01, 0x7d,       // T120 L4    D
  0x40, 0x1e, 0x01, 0x53,       // T120 L4    E
  0x40, 0x1e, 0x01, 0x40,       // T120 L4    F
  0x40, 0x1e, 0x01, 0x53,       // T120 L4    E
  0x40, 0x1e, 0x01, 0x7d,       // T120 L4    D
  0x40, 0x1e, 0x01, 0xac,       // T120 L4    C
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel B
const uint8_t bgm_part_B_ch_2[] = {
  0x60, 0x1e, 0x88, 0x00, 0xd6, // T120 L4 V8 O5C
  0x40, 0x1e, 0x00, 0xbe,       // T120 L4    O5D
  0x40, 0x1e, 0x00, 0xaa,       // T120 L4    O5E
  0x40, 0x1e, 0x00, 0xa0,       // T120 L4    O5F
  0x40, 0x1e, 0x00, 0xaa,       // T120 L4    O5E
  0x40, 0x1e, 0x00, 0xbe,       // T120 L4    O5D
  0x40, 0x1e, 0x00, 0xd6,       // T120 L4    O5C
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel C
const uint8_t bgm_part_B_ch_3[] = {
  0x60, 0x1e, 0x88, 0x02, 0x3b, // T120 L4 V8 O3G
  0x40, 0x1e, 0x01, 0xfd,       // T120 L4    O3A
  0x40, 0x1e, 0x01, 0xc5,       // T120 L4    O3B
  0x40, 0x1e, 0x01, 0xac,       // T120 L4    O4C
  0x40, 0x1e, 0x01, 0xc5,       // T120 L4    O3B
  0x40, 0x1e, 0x01, 0xfd,       // T120 L4    O3A
  0x40, 0x1e, 0x02, 0x3b,       // T120 L4    O3G
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};

// the sound fragment structure that holds pointers to the sound data streams.
const struct sound_fragment bgm_B_part = {
  .streams = {
    [0] = bgm_part_B_ch_1,
    [1] = bgm_part_B_ch_2,
    [2] = bgm_part_B_ch_3,
  },
};

// the list of "pointers" to the sound fragments.
const struct sound_fragment * bgm_fragments[] = {
  &bgm_A_part,                  // C D E R
  &bgm_A_part,                  // C D E R
  &bgm_B_part,                  // C D E F E D C R
};

// the sound clip structure.
const struct sound_clip bgm = {
  .num_fragments = ARRAY_SIZEOF(bgm_fragments),
  .fragments = bgm_fragments,
};

// ---- Sound effect ----

// sound data stream for PSG channel A
const uint8_t se_channel_1[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x00, 0xd6, // T180 L32 S0 M4096 O5C
  0x60, 0x03, 0x90, 0x00, 0xca,             // T180 L32 S0       O5C+
  0x60, 0x05, 0x90, 0x00, 0xbe,             // T180 L16 S0       O5D
  0xff,                                     // end of stream
};

// sound data stream for PSG channel B
const uint8_t se_channel_2[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x01, 0xac, // T180 L32 S0 M4096 O4C
  0x60, 0x03, 0x90, 0x01, 0x94,             // T180 L32 S0       O4C+
  0x60, 0x05, 0x90, 0x01, 0x7d,             // T180 L16 S0       O4D
  0xff,                                     // end of stream
};

// the sound fragment structure that holds pointers to the sound data streams.
const struct sound_fragment se_A_part = {
  .streams = {
    [0] = se_channel_1,
    [1] = se_channel_2,
    [2] = 0,                    // not used
  },
};

// the list of "pointers" to the sound fragments.
const struct sound_fragment * se_fragments[] = {
  &se_A_part,
};

// the sound clip structure.
const struct sound_clip se = {
  .priority = 0,
  .num_fragments = ARRAY_SIZEOF(se_fragments),
  .fragments = se_fragments,
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
