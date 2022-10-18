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

#define PSG(reg)                     (ay_3_8910_buffer[(reg)])

static uint8_t volume[3];       // main volume
static bool paused;
static bool repeat;
static uint8_t vsync_freq;
uint8_t snd_speed_multiplier;

struct snd_ctx snd_bgm;
struct snd_ctx snd_sfx;

static void snd__init_ctx(struct snd_ctx * ctx) {
  snd_m__init(&ctx->m);
  ctx->song_freq = vsync_freq;
  ctx->play_freq = vsync_freq;
  ctx->counter = 0;
}

static void snd__set_program(struct snd_ctx * ctx, const snd_Program * data) {
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
  repeat = status;
}

static void snd__init(void) {
  vsync_freq = msx_get_vsync_frequency();
  snd_speed_multiplier = SND_SPEED_1X;
  // psg_init();
  ay_3_8910_init();
  snd__pause();
  snd_i__set_i_tables(0, 0);
  snd_p__set_p_tables(0, 0);
  snd__init_ctx(&snd_sfx);
  snd__init_ctx(&snd_bgm);
}

#define DI() __asm__("di")
#define EI() __asm__("ei")

extern void snd__set_speed(uint8_t multiplier);

static void snd__set_bgm(const snd_Program * data) {
  snd__set_program(&snd_bgm, data);
  snd__set_speed(snd_speed_multiplier);
}

static void snd__stop(void) {
  const snd_Program * p = snd_bgm.m.music;
  snd__init();
  if (p) {
    snd__set_bgm(p);
  }
}

void snd_set_bgm(const snd_Program * data) {
  DI();
  snd__set_bgm(data);
  EI();
}

void snd_set_sfx(const snd_Program * data) {
  DI();
  snd__set_program(&snd_sfx, data);
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

static void snd__decode(struct snd_ctx * ctx);
static void snd__synthesis(void);

void snd_play(void) {
  if (paused) {
    return;
  }
  ay_3_8910_play();

  snd__decode(&snd_bgm);
  snd__decode(&snd_sfx);
  snd__synthesis();

  if (repeat && snd_bgm.m.isEnd) {
    uint8_t freq = snd_bgm.play_freq;
    if (snd_bgm.m.music) {
      snd__set_bgm(snd_bgm.m.music);
    }
    snd_bgm.play_freq = freq;
  }
}

// ------------------------------------------------

static void snd__decode(struct snd_ctx * ctx) {
  if (ctx->m.isEnd) return;
  uint16_t counter = ctx->counter + ctx->play_freq;
  while (vsync_freq <= counter) {
    counter -= vsync_freq;
    // ----
    snd_m__decode(&ctx->m);
    struct snd_channel * pch = &ctx->m.channels[0];
    for (uint8_t ch = 3; ch--; pch++) {
      snd_i__decode(&pch->i);
      snd_a__decode(&pch->a);
      snd_p__decode(&pch->p);
    }
  }
  ctx->counter = counter;
}

// ------------------------------------------------

// inline bool is_playing(struct snd_ctx * ctx, uint8_t ch) {
//   struct snd_channel * pch = &ctx->m.channels[ch];
//   return !ctx->m.isEnd && 0 <= pch->pitch && !!pch->volume;
// }

static void snd__synthesis(void) {
  PSG(7) = 0xb8;
  if (snd_sfx.m.isEnd) {
    struct snd_channel * pch = &snd_bgm.m.channels[2];
    for (uint8_t ch = 3; ch--; pch--) {
      snd_channel_synthesis(ch, pch);
    }
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
