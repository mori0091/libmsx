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

#include <assert.h>
#include <stdint.h>
#include <string.h>

struct SCC_buffer scc_buffer;
struct SCC_wave_buffer scc_wave_buffer;

void SCC_init(void) {
  memset(&scc_buffer, 0, sizeof(scc_buffer));
  memset(&scc_wave_buffer, 0, sizeof(scc_wave_buffer));
}

void SCC_stop(struct SCC * scc) {
  if (!scc->slot) return;
  const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
  msx_ENASLT(scc->slot, (void *)PAGE_ADDR(2));
  if (scc->mode == 1) {
    memset((void *)SCC_volume, 0, 5);
  }
  else if (scc->mode == 2) {
    memset((void *)SCCPlus_volume, 0, 5);
  }
  msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
}

void SCC_play(struct SCC * scc) {
  if (!scc->slot) return;
  const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
  msx_ENASLT(scc->slot, (void *)PAGE_ADDR(2));
  if (scc->mode == 1) {
    // write waveforms
    if (scc_wave_buffer.updated) {
      scc_wave_buffer.updated = 0;
      memcpy((void *)SCC_waveform, &scc_wave_buffer.waveform[0], 32 * 4);
    }
    // write fdr x 5 channels, volume x 5 channels, and channel_mask.
    memcpy((void *)SCC_fdr, &scc_buffer, sizeof(scc_buffer));
  }
  else if (scc->mode == 2) {
    // write waveforms
    if (scc_wave_buffer.updated) {
      scc_wave_buffer.updated = 0;
      memcpy((void *)SCCPlus_waveform, &scc_wave_buffer.waveform[0], 32 * 5);
    }
    // write fdr x 5 channels, volume x 5 channels, and channel_mask.
    memcpy((void *)SCCPlus_fdr, &scc_buffer, sizeof(scc_buffer));
  }
  msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
}

void SCC_set_waveform(uint8_t ch, const int8_t waveform[32]) {
  assert(ch < 5);
  memcpy(&scc_wave_buffer.waveform[ch], waveform, 32);
  scc_wave_buffer.updated |= 1 << ch;
}
