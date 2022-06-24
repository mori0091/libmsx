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
 * \file snd_m.h
 *
 */

#ifndef SND_M_H_
#define SND_M_H_

#include <stdint.h>
#include <stdbool.h>

#include "./snd_i.h"
#include "./snd_a.h"
#include "./snd_p.h"

struct snd_channel {
  struct snd_i_ctx i;           // context for instrument
  struct snd_a_ctx a;           // context for arpeggio
  struct snd_p_ctx p;           // context for pitch envelope
  // ----
  struct {
    int16_t pitch;              // (note number) << 8 ; i.e. [1/256 note]
    uint8_t volume;             // current volume level
    int8_t  arp;                // +/- note number
  };
  // ---- expression (fade-in, fade-out) ----
  struct {
    uint16_t fade_wait;
    uint16_t fade_timer;
    int8_t   fade;              // off (0), fade-in (+1), fade-out (-1)
    bool     fade_triggered;
  };
  // ---- expression (pitch bend) ----
  struct {
    uint16_t pitch_wait;
    uint16_t pitch_timer;
    int16_t  pitch_delta;       // off (0), +/- pitch
    bool     pitch_triggered;
  };
  struct {
    uint16_t pitch_glide;       // off (0), pitch
    uint16_t pitch_min;         // PITCH_MIN or glide target (be set on NoteOn message)
    uint16_t pitch_max;         // PITCH_MAX or glide target (be set on NoteOn message)
  };
};

struct snd_m_ctx {
  uint8_t timer;
  // ----
  const uint8_t * data;         // music stream
  const uint8_t * next;         // pointer to next octet of the stream
  // ----
  bool isEnd;
  // ----
  struct snd_channel channels[3];
};

void snd_m__init(struct snd_m_ctx * ctx);
void snd_m__program_change(struct snd_m_ctx * ctx, const uint8_t * m_stream);

uint8_t snd_m__stream_take(struct snd_m_ctx * ctx);
void snd_m__decode(struct snd_m_ctx * ctx);
void snd_m__synthesis(struct snd_channel * pchs[3]);

#endif // SND_M_H_
