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
#include "./snd_e.h"

struct snd_channel {
  struct snd_i_ctx i;           // context for instrument
  struct snd_a_ctx a;           // context for arpeggio
  struct snd_p_ctx p;           // context for pitch envelope
  struct snd_e_ctx e;           // context for amplitude envelope
  // ----
  struct {
    uint8_t note;               // note number
    uint8_t volume;             // current volume level
    int8_t  arp;                // +/- note number
  };
  // ---- expression (fade-in, fade-out) ----
  struct {
    uint16_t fade_wait;
    uint16_t fade_timer;
    int8_t   fade;              // constant (0) fade-in (+1), fade-out (-1)
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
