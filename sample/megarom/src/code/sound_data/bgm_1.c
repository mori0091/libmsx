// -*- coding: utf-8-unix -*-
/**
 * \file bgm_1.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
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

// ---- Background music ----
// ---- (A part) ----

// sound data stream for PSG channel A
static const uint8_t A_part_ch_1[] ={
  0x60, 0x1e, 0x88, 0x01, 0xac, // T120 L4 V8 C
  0x40, 0x1e, 0x01, 0x7d,       // T120 L4    D
  0x40, 0x1e, 0x01, 0x53,       // T120 L4    E
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel B
static const uint8_t A_part_ch_2[] = {
  0x60, 0x1e, 0x88, 0x00, 0xd6, // T120 L4 V8 O5C
  0x40, 0x1e, 0x00, 0xbe,       // T120 L4    O5D
  0x40, 0x1e, 0x00, 0xaa,       // T120 L4    O5E
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// sound data stream for PSG channel C
static const uint8_t A_part_ch_3[] = {
  0x60, 0x1e, 0x88, 0x02, 0x3b, // T120 L4 V8 O3G
  0x40, 0x1e, 0x01, 0xfd,       // T120 L4    O3A
  0x40, 0x1e, 0x01, 0xc5,       // T120 L4    O3B
  0x20, 0x1e, 0x10,             // T120 L4    R
  0xff,                         // end of stream
};
// the sound fragment structure that holds pointers to the sound data streams.
static const struct sound_fragment A_part = {
  .streams = {
    [0] = A_part_ch_1,
    [1] = A_part_ch_2,
    [2] = A_part_ch_3,
  },
};

// ---- (B part) ----

// sound data stream for PSG channel A
static const uint8_t B_part_ch_1[] ={
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
static const uint8_t B_part_ch_2[] = {
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
static const uint8_t B_part_ch_3[] = {
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
static const struct sound_fragment B_part = {
  .streams = {
    [0] = B_part_ch_1,
    [1] = B_part_ch_2,
    [2] = B_part_ch_3,
  },
};

// the list of "pointers" to the sound fragments.
static const struct sound_fragment * music[] = {
  &A_part,                  // C D E R
  &A_part,                  // C D E R
  &B_part,                  // C D E F E D C R
};

// the sound clip structure.
const struct sound_clip bgm_1 = {
  .num_fragments = sizeof(music) / sizeof(music[0]),
  .fragments = music,
};
