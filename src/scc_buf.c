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
 * \file scc_buf.c
 */

#include <bios.h>
#include <scc.h>
#include <scc_buf.h>
#include <scc_wav.h>
#include "scc_io.h"

#include <stdint.h>
#include <string.h>

struct SCC_buffer scc_buffer;

static struct {
  /** Update-bit for each of five channels. */
  uint8_t updated;
  /** Waveform for each of five channels. */
  int8_t waveform[5][32];
} scc_voice;

static void unexpose_SCC(void) {
  SCC_BANK_SELECT_3 = 0x00;
  SCCPlus_mode_select = 0x00;
  SCC_BANK_SELECT_2 = 0x00;
}
static void expose_SCC(void) {
  SCC_BANK_SELECT_3 = 0x00;
  SCCPlus_mode_select = 0x00;
  SCC_BANK_SELECT_2 = 0x3f;
}
static void expose_SCCPlus(void) {
  SCC_BANK_SELECT_3 = 0x00;
  SCCPlus_mode_select = 0x20;
  SCC_BANK_SELECT_3 = 0x80;
}

void SCC_init(void) {
  memset(&scc_buffer, 0, sizeof(scc_buffer));
  scc_buffer.channel_mask = 0x1f;
  for (uint8_t ch = 5; ch--; ) {
    SCC_set_waveform(ch, SCC_WAVEFORM_PULSE_1_32);
  }
}

void SCC_stop(struct SCC * scc) {
  if (!scc->slot) return;
  const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
  msx_ENASLT(scc->slot, (void *)PAGE_ADDR(2));
  if (scc->mode == 1) {
    expose_SCC();
    memset((void *)SCC_volume, 0, 5);
  }
  else /* if (scc->mode == 2) */ {
    expose_SCCPlus();
    memset((void *)SCCPlus_volume, 0, 5);
  }
  unexpose_SCC();
  msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
  // __asm__("ei");
}

void SCC_play(struct SCC * scc) {
  if (!scc->slot) return;
  const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
  msx_ENASLT(scc->slot, (void *)PAGE_ADDR(2));
  if (scc->mode == 1) {
    expose_SCC();
    // write waveforms
    if (scc_voice.updated) {
      scc_voice.updated = 0;
      memcpy((void *)SCC_waveform, &scc_voice.waveform[0], 32 * 4);
    }
    // write fdr x 5 channels, volume x 5 channels, and channel_mask.
    memcpy((void *)SCC_fdr, &scc_buffer, sizeof(scc_buffer));
  }
  else /* if (scc->mode == 2) */ {
    expose_SCCPlus();
    // write waveforms
    if (scc_voice.updated) {
      scc_voice.updated = 0;
      memcpy((void *)SCCPlus_waveform, &scc_voice.waveform[0], 32 * 5);
    }
    // write fdr x 5 channels, volume x 5 channels, and channel_mask.
    memcpy((void *)SCCPlus_fdr, &scc_buffer, sizeof(scc_buffer));
  }
  unexpose_SCC();
  msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
  // __asm__("ei");
}

void SCC_set_waveform(uint8_t ch, const int8_t waveform[32]) {
  if (5 <= ch) return;
  memcpy(&scc_voice.waveform[ch], waveform, 32);
  scc_voice.updated |= 1 << ch;
}
