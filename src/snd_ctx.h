// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file snd_ctx.h
 *
 */

#ifndef SND_CTX_H_
#define SND_CTX_H_

#include "./snd_sound.h"
#include "./snd_m.h"

#if defined(__SDCC)
#  define DI() __asm__("di")
#  define EI() __asm__("ei")
#else
#  define DI()
#  define EI()
#endif

struct snd_ctx {
  struct snd_m_ctx m;
  uint8_t song_freq;            // frequency of the song
};

extern struct snd_ctx snd_bgm;
extern struct snd_ctx snd_sfx;

#endif // SND_CTX_H_
