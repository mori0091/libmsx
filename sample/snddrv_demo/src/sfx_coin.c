// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file sfx_coin.c
 */

#include <snd_sound.h>

// ---- Sound effects ----
// -------------------------------------------------------------
#if 1
static const uint8_t stream[] = {
  //ch, expressions...,  note, i#, wait, // channel: note i# expressions         wait+1
  0x81, 0x90, 0x8f,        83,  3, 0x02, // PSG chB:  B_5 03 vF.. .... .... .... 03
  0x81,                    88,  3, 0x1f, // PSG chB:  B_6 03 .... .... .... .... 20
  0xff,                                  // EOM
};

const snd_Stream sfx_coin = {
  .replayRate = 60,             // 60Hz
  .stream = stream,
};

// -------------------------------------------------------------
#else

#define ARRAY_SIZEOF(a)   (sizeof(a)/sizeof(a[0]))
#define VEC_FROM_ARRAY(a) {.length = ARRAY_SIZEOF(a), .data = a}

// ---- track #1 -----------------------------------------------
static const uint8_t track1[] = {
//expr...   , note, i#, skip,
  0x90, 0x8f,   83,  3, 0xc0,   // line  1: B_5 03 vF.. .... .... ....
                88,  3, 0xca,   // line  2: E_6 03 .... .... .... ....
                                // line  3: (10 empty lines)
                                // line 12: (last line of track)
};
// ---- all tracks ---------------------------------------------
static const snd_Track tracks[] = {
  [0] = {0},                    /* null track */
  [1] = VEC_FROM_ARRAY(track1),
};

// ---- patterns -----------------------------------------------
static const snd_Pattern patterns[] = {
// height, {special channels}, {channels}
  {12,     {0},                {{0}, {1}, {0}}},
};

// ---- program ------------------------------------------------
const snd_Program sfx_coin = {
  .replayRate = 60,             // Hz
  .speedTracks = {0},           // no speed tracks
  .eventTracks = {0},           // no event tracks
  .tracks = VEC_FROM_ARRAY(tracks),
  .patterns = VEC_FROM_ARRAY(patterns),
  .initialSpeed = 3,            // ticks per line
  .isLoop = false,
  .loopToIndex = 0,
};

#endif
// -------------------------------------------------------------
