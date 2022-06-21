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

static uint8_t volume[3];       // main volume
static bool paused;
static bool repeat;
static uint8_t vsync_freq;
uint8_t snd_speed_multiplier4x;

struct snd_ctx snd_bgm;
struct snd_ctx snd_sfx;

static void snd__init_ctx(struct snd_ctx * ctx) {
  snd_m__init(&ctx->m);
  ctx->song_freq = 60;          // \TODO
  ctx->play_freq = ctx->song_freq;
  ctx->counter = 0;
}

static void snd__set_song(struct snd_ctx * ctx, void * data) {
  snd__init_ctx(ctx);
  snd_m__program_change(&ctx->m, data); // \TODO
}

static void snd__pause(void) {
  paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

static void snd__set_repeat(bool status) {
  repeat = status;
}

static void snd__init(void) {
  vsync_freq = msx_get_vsync_frequency();
  snd_speed_multiplier4x = SND_SPEED_1X;
  // psg_init();
  ay_3_8910_init();
  snd__pause();
  snd__init_ctx(&snd_sfx);
  snd__init_ctx(&snd_bgm);
}

static void snd__stop(void) {
  const uint8_t * song = snd_bgm.m.data;
  snd__init();
  snd__set_song(&snd_bgm, (void *)song);
}

#define DI() __asm__("di")
#define EI() __asm__("ei")

static void snd__set_bgm(void * data) {
  snd__set_song(&snd_bgm, data);
  snd_set_player_frequency((int)snd_speed_multiplier4x * snd_bgm.song_freq / 4);
}

void snd_set_bgm(void * data) {
  DI();
  snd__set_bgm(data);
  EI();
}

void snd_set_sfx(void * data) {
  (void)data;
  DI();
  snd__set_song(&snd_sfx, data);
  EI();
}

void snd_pause(void) {
  DI();
  snd__pause();
  EI();
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

inline void snd__process(struct snd_ctx * ctx);
inline void snd__mixing(void);

void snd_play(void) {
  if (paused) {
    return;
  }
  ay_3_8910_play();

  snd__process(&snd_bgm);
  snd__process(&snd_sfx);
  snd__mixing();

  if (repeat && snd_bgm.m.isEnd) {
    snd__set_bgm((void *)snd_bgm.m.data);
  }
}

// ------------------------------------------------

static void snd__decode(struct snd_ctx * ctx) {
  snd_m__decode(&ctx->m);
  uint8_t ch = 3;
  while (ch--) {
    struct snd_channel * pch = &ctx->m.channels[ch];
    snd_i__decode(&pch->i);
    snd_a__decode(&pch->a);
    snd_p__decode(&pch->p);
    snd_e__decode(&pch->e);
  }
}

inline void snd__synthesis(struct snd_ctx * ctx) {
  snd_m__synthesis(&ctx->m);
}

inline void snd__process(struct snd_ctx * ctx) {
  ctx->counter += ctx->play_freq;
  while (vsync_freq <= ctx->counter) {
    ctx->counter -= vsync_freq;
    snd__decode(ctx);
    snd__synthesis(ctx);
  }
}

// ------------------------------------------------

inline bool is_playing(struct snd_ctx * ctx, uint8_t ch) {
  return !!ctx->m.channels[ch].volume;
}

inline void snd__mixing(void) {
  for (uint8_t ch = 0; ch < 3; ++ch) {
    snd_m__mixing(is_playing(&snd_sfx, ch) ? &snd_sfx.m : &snd_bgm.m, ch);
  }
}
