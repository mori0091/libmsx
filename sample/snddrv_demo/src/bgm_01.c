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
 * \file bgm_01.c
 * A music data example of snd_Program type for snddrv.
 */

#include <snd_sound.h>

#define ARRAY_SIZEOF(a)   (sizeof(a)/sizeof(a[0]))
#define VEC_FROM_ARRAY(a) {.length = ARRAY_SIZEOF(a), .data = a}
#define EOL               (0xc0)

// ---- speed tracks -------------------------------------------
static const uint8_t speedTrack0[] = {
  4,
};
static const uint8_t speedTrack1[] = {
  4, 4, 4, 1, 1, 0,
  9, 9,
};
static const uint8_t speedTrack2[] = {
  9,
};

// ---- all speed tracks ---------------------------------------
static const snd_SpeedTrack speedTracks[] = {
  [0] = VEC_FROM_ARRAY(speedTrack0),
  [1] = VEC_FROM_ARRAY(speedTrack1),
  [2] = VEC_FROM_ARRAY(speedTrack2),
};

// ---- track #1 -----------------------------------------------
static const uint8_t track1[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8f, 0x15, 0x70,       60,  1, EOL,  // PSG chA:  C_4 01 vF.. .... b57. .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                60,  1, EOL,  // PSG chA:  C_4 01 .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
  0x90, 0x70, 0x80,             62,  1, EOL,  // PSG chA:  D_4 01 .... g080 .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                62,  1, EOL,  // PSG chA:  D_4 01 .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
  0x90, 0x70, 0x80,             60,  1, EOL,  // PSG chA:  C_4 01 .... g080 .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                60,  1, EOL,  // PSG chA:  C_4 01 .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
};
static const uint8_t track1b[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
                                55,  1, EOL,  // PSG chA:  G_3 01 .... .... .... .... 05
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 05
  0x80, 0xa0, 0x80,                     EOL,  // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x90, 0x80,                     EOL,  // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0xa0, 0x80,                     EOL,  // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x90, 0x80,                     EOL,  // PSG chA:  ... .. i080 .... .... .... 01
  0x80, 0xa0, 0x80,                     EOL,  // PSG chA:  ... .. o080 .... .... .... 0a
                                        EOL,  // PSG chA:  ... .. .... .... .... .... 0a
};
// ---- track #2 -----------------------------------------------
static const uint8_t track2[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x2c, 0x0c,       67,  1, EOL,  // PSG chB:  G_4 01 vC.. .... cC0C .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
  0x90, 0x8d,                   67,  2, EOL,  // PSG chB:  G_4 02 vD.. .... .... .... 05
                                69,  2, EOL,  // PSG chB:  A_4 02 .... .... .... .... 05
                                67,  2, EOL,  // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, EOL,  // PSG chB:  A_4 02 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
  0x90, 0x8e,                   67,  1, EOL,  // PSG chB:  G_4 01 vE.. .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  2, EOL,  // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  2, EOL,  // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
};
static const uint8_t track2b[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x90, 0x8f,                   67,  1, EOL,  // PSG chB:  G_4 01 vF.. .... .... .... 05
                                69,  2, EOL,  // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, EOL,  // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, EOL,  // PSG chB:  A_4 01 .... .... .... .... 02
                                        EOL,  // PSG chB:  ... .. .... .... .... .... 02
                                        EOL,  // PSG chB:  ... .. .... .... .... .... 01
                                76,  1, EOL,  // PSG chB:  E_5 01 .... .... .... .... 0a
  0x81, 0xa0, 0x08, 0x20, 0x0c,         EOL,  // PSG chB:  ... .. o008 .... c00C .... 0a
};
// ---- track #3 -----------------------------------------------
static const uint8_t track3[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x61, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vC.. f100 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8f, 0x62, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 vF.. f200 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8d, 0x63, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vD.. f300 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x64, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 .... f400 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x65, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 .... f500 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8c, 0x61, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vC.. f100 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8f, 0x62, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 vF.. f200 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
};
static const uint8_t track3b[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8d, 0x63, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vD.. f300 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x64, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 .... f400 .... .... 05
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 02
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 02
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 01
  0x90,       0x65, 0x00,       71,  2, EOL,  // PSG chC:  B_4 02 .... f500 .... .... 0a
                                        EOL,  // PSG chC:  ... .. .... .... .... .... 0a
};
// ---- track #4 -----------------------------------------------
static const uint8_t track4[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x71, 0x00,       50,  1, EOL,  // PSG chA:  D_3 01 vC.. g100 .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, EOL,  // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
                                48,  1, EOL,  // PSG chA:  C_3 01 .... .... .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
                                48,  1, EOL,  // PSG chA:  C_3 01 .... .... .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, EOL,  // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, EOL,  // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     EOL,  // PSG chA:  OFF .. .... .... .... .... 0a
};
static const uint8_t track4b[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
                                55,  1, EOL,  // PSG chA:  G_3 01 .... .... .... .... 05
                                        EOL,  // PSG chA   ... .. .... .... .... .... 05
  0x80, 0xa0, 0x80,                     EOL,  // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x90, 0x80,                     EOL,  // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0xa0, 0x80,                     EOL,  // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x90, 0x80,                     EOL,  // PSG chA:  ... .. i080 .... .... .... 01
                                        EOL,  // PSG chA   ... .. .... .... .... .... 0a
                                        EOL,  // PSG chA   ... .. .... .... .... .... 0a
};
// ---- track #5 -----------------------------------------------
// static const uint8_t track5[] = {
// };
// ---- track #6 -----------------------------------------------
static const uint8_t track6[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x61, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vC.. f100 .... .... 0a
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8f, 0x62, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 vF.. f200 .... .... 0a
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8d, 0x63, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vD.. f300 .... .... 0a
  0x90,       0x64, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 .... f400 .... .... 0a
  0x90,       0x65, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 .... f500 .... .... 0a
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8c, 0x61, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vC.. f100 .... .... 0a
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8f, 0x62, 0x00,       59,  2, EOL,  // PSG chC:  B_3 02 vF.. f200 .... .... 0a
                                 0,     EOL,  // PSG chC:  OFF .. .... .... .... .... 0a
};
static const uint8_t track6b[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8d, 0x63, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 vD.. f300 .... .... 05
                                        EOL,  // PSG chC   ... .. .... .... .... .... 05
  0x90,       0x64, 0x00,       36,  2, EOL,  // PSG chC:  C_2 02 .... f400 .... .... 05
                                        EOL,  // PSG chC   ... .. .... .... .... .... 02
                                        EOL,  // PSG chC   ... .. .... .... .... .... 02
                                        EOL,  // PSG chC   ... .. .... .... .... .... 01
  0x90,       0x65, 0x00,       71,  2, EOL,  // PSG chC:  B_4 02 .... f500 .... .... 0a
                                        EOL,  // PSG chC   ... .. .... .... .... .... 0a
};

// ---- all tracks ---------------------------------------------
static const snd_Track tracks[] = {
  [0] = {0},                    // an empty track
  [1] = VEC_FROM_ARRAY(track1),
  [2] = VEC_FROM_ARRAY(track2),
  [3] = VEC_FROM_ARRAY(track3),
  [4] = VEC_FROM_ARRAY(track1b),
  [5] = VEC_FROM_ARRAY(track2b),
  [6] = VEC_FROM_ARRAY(track3b),
  [7] = VEC_FROM_ARRAY(track4),
  [8] = VEC_FROM_ARRAY(track6),
  [9] = VEC_FROM_ARRAY(track4b),
  [10] = VEC_FROM_ARRAY(track6b),
};

// ---- patterns -----------------------------------------------
static const snd_Pattern patterns[] = {
//height, sp, ev,   chA,  chB,  chC
  {   24, {0, 0}, {{ 1}, { 2}, { 3}}},
  {    8, {1, 0}, {{ 4}, { 5}, { 6}}},
  {   24, {0, 0}, {{ 1}, { 2, 0xc00}, { 3}}},
  {    8, {1, 0}, {{ 4}, { 5, 0xc00}, { 6}}},
  {   12, {2, 0}, {{ 7}, { 0}, { 8}}},
  {    8, {1, 0}, {{ 9}, { 0}, {10}}},
  {   12, {2, 0}, {{ 7}, { 0}, { 8}}},
  {    8, {1, 0}, {{ 9}, { 0}, {10}}},
};

// ---- program ------------------------------------------------
const snd_Program bgm_01 = {
  .replayRate   = 60,           // Hz
  .speedTracks  = VEC_FROM_ARRAY(speedTracks),
  .eventTracks  = {0},          // no event tracks
  .tracks       = VEC_FROM_ARRAY(tracks),
  .patterns     = VEC_FROM_ARRAY(patterns),
  .initialSpeed = 0,            // Wait count per line (in ticks)
  .isLoop       = true,
  .loopToIndex  = 0,
};
