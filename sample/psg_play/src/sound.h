// -*- coding: utf-8-unix -*-

#ifndef SOUND_H
#define SOUND_H

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SOUND_CHANNEL_A   (1 << 0)
#define SOUND_CHANNEL_B   (1 << 1)
#define SOUND_CHANNEL_C   (1 << 2)
#define SOUND_CHANNEL_ALL (SOUND_CHANNEL_A | SOUND_CHANNEL_B | SOUND_CHANNEL_C)

// stream = chunk* eom
// chunk  = header body+
// eom    = 1111b 1111b
//
// header = body_length:3 duration_hi:5 duration_lo:8
// body   = tone
//        | volume
//        | envelope_pattern
//        | envelope_cycle
//        | envelope_pattern_and_cycle
//        | noise
//        | mixer
//
// tone   = 0000b fdr_hi:4 fdr_lo:8
// noise  = 001b fdr:5
// mixer  = 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
// volume = 1000b vol:4
// envelope_pattern
//        = 1001b pat:4
// envelope_cycle
//        = 0100b 0000b fdr_hi:8 fdr_lo:8
// envelope_pattern_and_cycle
//        = 1101b pat:4 fdr_hi:8 fdr_lo:8

struct sound_clip {
  uint8_t* streams[3];
};

void sound_set_repeat(bool repeat);

void sound_set_mute(uint8_t mute);

void sound_start(struct sound_clip* s);

void sound_stop(void);

void sound_pause(void);

void sound_player(void);

#endif
