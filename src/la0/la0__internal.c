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
 * \file la0__internal.c
 */

#include "la0__internal.h"
#include "BitReader.h"

#include <audio_buf.h>
#include <string.h>

struct la0 la0_;

static bool la0__bgm_decode_update(void);
static bool la0__sfx_decode_update(void);
static void la0__bgm_set_repeat(bool repeat);

// -----------------------------------------------------------------------

const AudioDecoder LA0_BGM_DECODER = {
  .decode_update = la0__bgm_decode_update,
  // .decode_final  = NULL,
  .set_repeat    = la0__bgm_set_repeat,
};

const AudioDecoder LA0_SFX_DECODER = {
  .decode_update = la0__sfx_decode_update,
  // .decode_final  = NULL,
  // .set_repeat    = NULL,
};

uint16_t la0_get_bgm_total_samples(void) {
  return la0_.bgm.total_samples;
}

uint16_t la0_get_bgm_loop_samples(void) {
  return la0_.bgm.loop_samples;
}

uint8_t la0_get_bgm_loop_counter(void) {
  return la0_.bgm.loop_counter;
}

// -----------------------------------------------------------------------

static uint8_t priority;        ///< 0: for BGM, 1: for SFX.

static uint8_t len;             ///< number of commands in the next sample.
static uint8_t buf[256];        ///< read-buffer for the next sample.

static bool la0__decode_update(struct la0_ctx * ctx);

static bool la0__bgm_decode_update(void) {
  priority = 0;
  return la0__decode_update(&la0_.bgm);
}

static bool la0__sfx_decode_update(void) {
  priority = 1;
  return la0__decode_update(&la0_.sfx);
}

static void la0__bgm_set_repeat(bool repeat) {
  la0_.bgm.repeat = repeat;
}

static void la0__lengh_of_next_sample(struct la0_song_ctx * song) {
  len = BitReader_read_elias_gamma_u8(&song->state.in) - 1;
}

static void la0__seek_to_next_sample(struct la0_song_ctx * song) {
  int32_t x = (int32_t)BitReader_read_elias_gamma_u32(&song->state.in);
  // x = 2 * |n| + 1 if n >= 0
  // x = 2 * |n|     if n < 0
  if (x == 1) {
    // offset = 2n = 0
    return;
  }
  else if (x & 1) {
    // offset = 2n = x-1
    x--;
  }
  else {
    // offset = 2n = -x
    x = -x;
  }
  mfseek(&song->state.tbl, x, MEM_SEEK_CUR);
}

static void la0__push_commands(uint8_t n, MemFile * sccwav) {
  const uint8_t * p = buf;
  while (n--) {
    const uint8_t cmd = *p++;
    uint8_t val = *p++;
    if (priority) {
      if (0xaa <= cmd && cmd < 0xaf) {
        if (!(la0_.sfx.scc_channel_mask & (1 << (cmd - 0xaa)))) {
          continue;
        }
      }
      else if (cmd == 0xaf) {
        val |= (audio_buf_cache[0xaf] & ~la0_.sfx.scc_channel_mask);
        val &= 0x1f;
      }
      else if (cmd == 0xb7) {
        val &= (audio_buf_cache[0xb7] | la0_.sfx.psg_channel_mask);
        val = (val & 0xbf) | 0x80;
      }
      else if (0xb8 <= cmd && cmd < 0xbb) {
        if (!(la0_.sfx.psg_channel_mask & (9 << (cmd - 0xb8)))) {
          continue;
        }
      }
    }
    if (cmd < 0xfa) {
      audio_buf_put(priority, cmd, val);
    }
    else if (cmd < 0xff) {
      // SCC/SCC+ waveform
      const uint8_t ch = cmd - 0xfa;
      mfseek(sccwav, 32L * val, MEM_SEEK_SET);
      mfread(sccwav, &audio_buf_cache[ch*32], 32);
      audio_buf_put(priority, 0xfa, ch);
    }
  }
}

static bool la0__decode_update(struct la0_ctx * ctx) {
  if (ctx->loop_samples) {
    if (ctx->remaining_samples == ctx->loop_samples) {
      // save checkpoint
      ctx->song.checkpoint = ctx->song.state;
    }
    else if (!ctx->remaining_samples && ctx->repeat) {
      // restore checkpoint
      ctx->song.state = ctx->song.checkpoint;
      ctx->remaining_samples = ctx->loop_samples;
      ctx->loop_counter++;
    }
  }

  if (!ctx->remaining_samples) return false;
  ctx->remaining_samples--;

  struct la0_song_ctx * song = &ctx->song;
  la0__lengh_of_next_sample(song);
  if (len) {
    la0__seek_to_next_sample(song);
    uint8_t n = sizeof(buf)/2;
    while (len) {
      if (len < sizeof(buf)/2) {
        n = len;
      }
      len -= n;
      mfread(&song->state.tbl, buf, 2*n);
      la0__push_commands(n, &song->wav);
    }
  }

  return true;
}
