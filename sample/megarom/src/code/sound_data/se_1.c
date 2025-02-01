// -*- coding: utf-8-unix -*-
/**
 * \file se_1.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

// #pragma codeseg CODE

#include <sound.h>

// see also
// - https://mori0091.github.io/libmsx/sound_8h.html
// - https://mori0091.github.io/libmsx/structsound__clip.html
// - https://mori0091.github.io/libmsx/structsound__fragment.html

// ---- Sound effect #1 ----
// ---- (A part) ----

// sound data stream for PSG channel A
static const uint8_t A_part_ch_1[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x00, 0xd6, // T180 L32 S0 M4096 O5C
  0x60, 0x03, 0x90, 0x00, 0xca,             // T180 L32 S0       O5C+
  0x60, 0x05, 0x90, 0x00, 0xbe,             // T180 L16 S0       O5D
  0xff,                                     // end of stream
};

// sound data stream for PSG channel B
static const uint8_t A_part_ch_2[] = {
  0xa0, 0x02, 0xd0, 0x10, 0x00, 0x01, 0xac, // T180 L32 S0 M4096 O4C
  0x60, 0x03, 0x90, 0x01, 0x94,             // T180 L32 S0       O4C+
  0x60, 0x05, 0x90, 0x01, 0x7d,             // T180 L16 S0       O4D
  0xff,                                     // end of stream
};

// the sound fragment structure that holds pointers to the sound data streams.
static const struct sound_fragment A_part = {
  .streams = {
    [0] = A_part_ch_1,
    [1] = A_part_ch_2,
    [2] = 0,                    // not used
  },
};

// the list of "pointers" to the sound fragments.
const struct sound_fragment * music[] = {
  &A_part,
};

// the sound clip structure.
const struct sound_clip se_1 = {
  .priority = 0,
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
