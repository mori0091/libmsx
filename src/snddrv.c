// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
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

#include <audio.h>
#include <audio_cb.h>
#include <audio_dec.h>

static bool snd_bgm_decode_update(void);
static void snd_bgm_decode_final(void);
static void snd__set_repeat(bool status);

static bool snd_sfx_decode_update(void);
static void snd_sfx_decode_final(void);

const AudioDecoder SND_BGM_DECODER = {
  .decode_update = snd_bgm_decode_update,
  .decode_final  = snd_bgm_decode_final,
  .set_repeat = snd__set_repeat,
};

const AudioDecoder SND_SFX_DECODER = {
  .decode_update = snd_sfx_decode_update,
  .decode_final  = snd_sfx_decode_final,
  .set_repeat = NULL,
};

uint8_t snd_speed_multiplier = SND_SPEED_1X;

struct snd_ctx snd_bgm;
struct snd_ctx snd_sfx;
static uint8_t sfx_priority;

static void snd__set_program(struct snd_ctx * ctx, const snd_Music * data) {
  snd_m__init(&ctx->m);
  snd_m__program_change(&ctx->m, data);
  if (data && data->replayRate) {
    ctx->song_freq = data->replayRate;
  }
  else {
    ctx->song_freq = msx_get_vsync_frequency();
  }
}

static void snd__set_repeat(bool status) {
  snd_bgm.m.repeat = status;
}

extern void snd__set_speed(uint8_t multiplier);

static void snd__set_bgm(const snd_Music * data) {
  snd__set_program(&snd_bgm, data);
  snd__set_speed(snd_speed_multiplier);
  audio_cb_bgm_changed(data->replayRate, &SND_BGM_DECODER);
}

static void snd__set_sfx(const snd_Music * data) {
  snd__set_program(&snd_sfx, data);
  audio_cb_sfx_changed(data->replayRate, &SND_SFX_DECODER);
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
  snd__set_sfx(&sa->musics.data[index]);
  EI();
}

bool snd_is_playing(void) {
  return audio_is_playing();
}

bool snd_is_playing_bgm(void) {
  return audio_is_playing_bgm();
}

bool snd_is_playing_sfx(void) {
  return audio_is_playing_sfx();
}

void snd_pause(void) {
  audio_pause();
}

bool snd_is_paused(void) {
  return audio_is_paused();
}

void snd_set_repeat(bool repeat) {
  audio_set_repeat(repeat);
}

void snd_start(void) {
  audio_start();
}

void snd_stop(void) {
  audio_stop();
}

void snd_init(void) {
  audio_init();
}

void snd_play(void) {
  audio_play();
}

// ------------------------------------------------

static void snd__iap_decode(struct snd_channel * pch) {
  snd_i__decode(&pch->i);
  snd_a__decode(&pch->a);
  snd_p__decode(&pch->p);
}

static bool snd__decode_once(struct snd_m_ctx * ctx) {
  if (ctx->isEnd) return false;
  snd_m__decode(ctx);
  struct snd_channel * pch = &ctx->channels[0];
  snd__iap_decode(pch++);
  snd__iap_decode(pch++);
  snd__iap_decode(pch);
  return true;
}

static bool snd_bgm_decode_update(void) {
  return snd__decode_once(&snd_bgm.m);
}

static bool snd_sfx_decode_update(void) {
  return snd__decode_once(&snd_sfx.m);
}

static void snd_bgm_decode_final(void) {
  ay_3_8910_buffer[7] = 0xb8;
  snd_channel_synthesis(0, &snd_bgm.m.channels[0]);
  snd_channel_synthesis(1, &snd_bgm.m.channels[1]);
  snd_channel_synthesis(2, &snd_bgm.m.channels[2]);
}

static void snd_sfx_decode_final(void) {
  for (uint8_t ch = 3; ch--;) {
    struct snd_channel * pch = &snd_sfx.m.channels[ch];
    if (0 <= pch->pitch && pch->volume) {
      ay_3_8910_buffer[7] &= ~(0x09 << ch);
      ay_3_8910_buffer[7] |= (0x08 << ch);
      snd_channel_synthesis(ch, pch);
    }
  }
}
