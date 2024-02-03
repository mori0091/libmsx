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
 * \file la0__open.c
 */

#include "la0__internal.h"
#include "memfile.h"

#include <stdint.h>
#include <string.h>

static const uint8_t TAG_LA0_FILE[4] = {'L', 'A', '0', ' '};

static const uint8_t TAG_SCC_WAVETBL[4] = {'S', 'C', 'C', 'W'};
static const uint8_t TAG_STREAM_DATA[4] = {'S', 'T', 'R', 'M'};

struct TLV_HEADER {
  uint8_t tag[4];
  uint32_t length;
};

int la0__count_songs(MemFile * mf) {
  if (!mf || !mf->methods) return 0;

  mfseek(mf, 0, MEM_SEEK_SET);

  struct TLV_HEADER header;

  int n = 0;
  for (;;) {
    if (mfeof(mf)) return n;
    mfread(mf, &header, sizeof(header));
    if (memcmp(header.tag, TAG_LA0_FILE, 4)) return n;
    mfseek(mf, header.length, MEM_SEEK_CUR);
    n++;
  }
}

static bool la0__get_song(MemFile * mf, struct la0_ctx * ctx) {
  struct TLV_HEADER header;
  __asm__("di");
  memset(ctx, 0, sizeof(struct la0_ctx));
  ctx->rate = mfread_u8(mf);               // 1 byte
  ctx->soundchip_enable = mfread_u8(mf);   // 1 byte
  ctx->total_samples = mfread_u16(mf);     // 2 byte
  ctx->loop_samples = mfread_u16(mf);      // 2 byte
  {
    uint8_t mask = mfread_u8(mf);
    ctx->scc_channel_mask = (mask >> 3);
    mask &= 7;
    ctx->psg_channel_mask = (mask << 3) | mask;
  }
  mfseek(mf, 1, MEM_SEEK_CUR);             // 1 byte (padding)

  ctx->remaining_samples = ctx->total_samples;

  while (!mfeof(mf)) {
    mfread(mf, &header, sizeof(header));
    if (!memcmp(header.tag, TAG_SCC_WAVETBL, 4)) {
      mfslice(&ctx->song.wav, mf, header.length);
    }
    else if (!memcmp(header.tag, TAG_STREAM_DATA, 4)) {
      MemFile mf2;
      mfslice(&mf2, mf, header.length);
      uint32_t len = mfread_u32(&mf2);
      mfslice(&ctx->song.state.in.mf, &mf2, len);
      mfseek(&mf2, len, MEM_SEEK_CUR);
      mfslice(&ctx->song.state.tbl, &mf2, header.length - len - 4);
    }
    else {
      memset(ctx, 0, sizeof(struct la0_ctx));
      return false;
    }
    mfseek(mf, header.length, MEM_SEEK_CUR);
  }
  return true;
}

bool la0__open(uint8_t idx, MemFile * mf, struct la0_ctx * ctx) {
  if (!mf || !mf->methods || !ctx) return false;

  mfseek(mf, 0, MEM_SEEK_SET);

  struct TLV_HEADER header;

  while (idx--) {
    if (mfeof(mf)) return false;
    mfread(mf, &header, sizeof(header));
    if (memcmp(header.tag, TAG_LA0_FILE, 4)) return false;
    mfseek(mf, header.length, MEM_SEEK_CUR);
  }

  mfread(mf, &header, sizeof(header));
  if (memcmp(header.tag, TAG_LA0_FILE, 4)) return false;

  MemFile song;
  mfslice(&song, mf, header.length);
  return la0__get_song(&song, ctx);
}
