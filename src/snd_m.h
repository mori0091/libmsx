// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
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

#include "../include/snd_sound.h"
#include "./snd_channel.h"

struct snd_u8_ctx {
  uint8_t skip_count;           // number of subsequent lines to skip
  const uint8_t * next;         // pointer to the next octet of the stream.
};

struct snd_m_ctx {
  uint8_t wait;
  uint8_t timer;
  bool isEnd;
  bool repeat;
  // ----
  struct {                      // Sequencer for snd_Music
    const snd_SoundAssets * sa; // pointer to the current sound assets
    const snd_Music * music;    // pointer to the current music
    uint8_t pindex;             // current pattern index (0..)
    uint8_t line;               // current line number (0..height-1)
    struct snd_u8_ctx spd;      // context for speed track
  };
  // ----
  struct snd_channel channels[3];
};

void snd_m__init(struct snd_m_ctx * ctx);
void snd_m__program_change(struct snd_m_ctx * ctx, const snd_Music * pg);
void snd_m__decode(struct snd_m_ctx * ctx);

#endif // SND_M_H_
