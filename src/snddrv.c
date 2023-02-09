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
 * \file snddrv.c
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "../include/bios.h"
#include "../include/psg.h"
#include "../include/ay_3_8910.h"

#include "../include/snddrv.h"
#include "./snd_ctx.h"

#define PSG(reg)                     (ay_3_8910_buffer[(reg)])

static bool paused;
static uint8_t vsync_freq;
uint8_t snd_speed_multiplier;

struct snd_ctx snd_bgm;
struct snd_ctx snd_sfx;
static uint8_t sfx_priority;

static void snd__init_ctx(struct snd_ctx * ctx) {
  snd_m__init(&ctx->m);
  ctx->song_freq = vsync_freq;
  ctx->play_freq = vsync_freq;
  ctx->counter = 0;
}

static void snd__set_program(struct snd_ctx * ctx, const snd_Music * data) {
  snd__init_ctx(ctx);
  snd_m__program_change(&ctx->m, data);
  if (data && data->replayRate) {
    ctx->song_freq = ctx->play_freq = data->replayRate;
  }
}

static void snd__pause(void) {
  paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

static void snd__set_repeat(bool status) {
  snd_bgm.m.repeat = status;
}

static void snd__init(void) {
  vsync_freq = msx_get_vsync_frequency();
  snd_speed_multiplier = SND_SPEED_1X;
  // psg_init();
  ay_3_8910_init();
  snd__pause();
  snd__init_ctx(&snd_sfx);
  snd__init_ctx(&snd_bgm);
}

extern void snd__set_speed(uint8_t multiplier);

static void snd__set_bgm(const snd_Music * data) {
  snd__set_program(&snd_bgm, data);
  snd__set_speed(snd_speed_multiplier);
}

static void snd__stop(void) {
  const snd_Music * p = snd_bgm.m.music;
  snd__init();
  if (p) {
    snd__set_bgm(p);
  }
}

void snd_set_bgm(uint8_t index, const snd_SoundAssets * sa) {
  DI();
  snd_bgm.m.sa = sa;
  snd__set_bgm(&sa->musics.data[index]);
  EI();
}

void snd_set_sfx(uint8_t index, const snd_SoundAssets * sa) {
  snd_set_sfx_with_priority(index, sa, 0);
}

void snd_set_sfx_with_priority(uint8_t index, const snd_SoundAssets * sa, uint8_t priority) {
  if (snd_is_playing_sfx() && priority < sfx_priority) {
    return;
  }
  DI();
  sfx_priority = priority;
  snd_sfx.m.sa = sa;
  snd__set_program(&snd_sfx, &sa->musics.data[index]);
  EI();
}

bool snd_is_playing(void) {
  return snd_is_playing_bgm() || snd_is_playing_sfx();
}

bool snd_is_playing_bgm(void) {
  return !snd_bgm.m.isEnd;
}

bool snd_is_playing_sfx(void) {
  return !snd_sfx.m.isEnd;
}

void snd_pause(void) {
  DI();
  snd__pause();
  EI();
}

bool snd_is_paused(void) {
  return paused;
}

void snd_set_repeat(bool repeat) {
  DI();
  snd__set_repeat(repeat);
  EI();
}

void snd_start(void) {
  DI();
  paused = false;
  EI();
}

void snd_stop(void) {
  DI();
  snd__stop();
  EI();
}

void snd_init(void) {
  DI();
  snd__init();
  EI();
}

static void snd__decode(struct snd_ctx * ctx);
static void snd__synthesis(void);

void snd_decode(void) {
  snd__decode(&snd_bgm);
  snd__decode(&snd_sfx);
  snd__synthesis();
}

void snd_play(void) {
  if (paused) {
    return;
  }
  ay_3_8910_play();
  snd_decode();
}

// ------------------------------------------------

static void snd__iap_decode(struct snd_channel * pch) {
  snd_i__decode(&pch->i);
  snd_a__decode(&pch->a);
  snd_p__decode(&pch->p);
}

static void snd__decode(struct snd_ctx * ctx) {
  if (ctx->m.isEnd) return;
  ctx->counter += ctx->play_freq;
  while (vsync_freq <= ctx->counter) {
    ctx->counter -= vsync_freq;
    // ----
    snd_m__decode(&ctx->m);
    struct snd_channel * pch = &ctx->m.channels[0];
    snd__iap_decode(pch++);
    snd__iap_decode(pch++);
    snd__iap_decode(pch);
  }
}

// ------------------------------------------------

// static inline bool is_playing(struct snd_ctx * ctx, uint8_t ch) {
//   struct snd_channel * pch = &ctx->m.channels[ch];
//   return !ctx->m.isEnd && 0 <= pch->pitch && !!pch->volume;
// }

static void snd__synthesis(void) {
  PSG(7) = 0xb8;
  if (snd_sfx.m.isEnd) {
    struct snd_channel * pch = &snd_bgm.m.channels[0];
    uint8_t ch = 0;
    snd_channel_synthesis(ch++, pch++);
    snd_channel_synthesis(ch++, pch++);
    snd_channel_synthesis(ch, pch);
  }
  else {
    for (uint8_t ch = 3; ch--;) {
      struct snd_channel * pch = &snd_sfx.m.channels[ch];
      if (pch->pitch < 0 || !pch->volume) {
        pch = &snd_bgm.m.channels[ch];
      }
      snd_channel_synthesis(ch, pch);
    }
  }
}
