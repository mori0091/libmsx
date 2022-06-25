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

#include "./snd_channel.h"

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
void snd_m__decode(struct snd_m_ctx * ctx);

#endif // SND_M_H_
