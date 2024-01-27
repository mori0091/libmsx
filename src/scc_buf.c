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

#define SCC_UPDATE_WAVEFORM(ch)                   \
  do {                                            \
    if (scc_wave_buffer.updated & (1 << (ch))) {  \
      memcpy((void *)&SCC_waveform[ch],           \
             &scc_wave_buffer.waveform[ch], 32);  \
    }                                             \
  } while (0)

#define SCC_UPDATE_FDR(ch)                        \
  SCC_fdr[ch] = scc_buffer.fdr[ch]

#define SCC_UPDATE_VOLUME(ch)                     \
  SCC_volume[ch] = scc_buffer.volume[ch]

#define SCC_UPDATE_MASK(ch)                       \
  SCC_channel_mask                                \
    = (SCC_channel_mask & ~(1 << (ch)))           \
    | (scc_buffer.channel_mask & (1 << (ch)))

static void SCC_update_registers(void) {
  SCC_UPDATE_WAVEFORM(0); SCC_UPDATE_FDR(0); SCC_UPDATE_VOLUME(0); SCC_UPDATE_MASK(0);
  SCC_UPDATE_WAVEFORM(1); SCC_UPDATE_FDR(1); SCC_UPDATE_VOLUME(1); SCC_UPDATE_MASK(1);
  SCC_UPDATE_WAVEFORM(2); SCC_UPDATE_FDR(2); SCC_UPDATE_VOLUME(2); SCC_UPDATE_MASK(2);
  SCC_UPDATE_WAVEFORM(3); SCC_UPDATE_FDR(3); SCC_UPDATE_VOLUME(3); SCC_UPDATE_MASK(3);
  /* no dedicated reg. */ SCC_UPDATE_FDR(4); SCC_UPDATE_VOLUME(4); SCC_UPDATE_MASK(4);
  scc_wave_buffer.updated = 0;
}

#define SCCI_UPDATE_WAVEFORM(ch)                  \
  do {                                            \
    if (scc_wave_buffer.updated & (1 << (ch))) {  \
      memcpy((void *)&SCCPlus_waveform[ch],       \
             &scc_wave_buffer.waveform[ch], 32);  \
    }                                             \
  } while (0)

#define SCCI_UPDATE_FDR(ch)                       \
  SCCPlus_fdr[ch] = scc_buffer.fdr[ch]

#define SCCI_UPDATE_VOLUME(ch)                    \
  SCCPlus_volume[ch] = scc_buffer.volume[ch]

#define SCCI_UPDATE_MASK(ch)                      \
  SCCPlus_channel_mask                            \
    = (SCCPlus_channel_mask & ~(1 << (ch)))       \
    | (scc_buffer.channel_mask & (1 << (ch)))

static void SCCI_update_registers(void) {
  SCCI_UPDATE_WAVEFORM(0); SCCI_UPDATE_FDR(0); SCCI_UPDATE_VOLUME(0); SCCI_UPDATE_MASK(0);
  SCCI_UPDATE_WAVEFORM(1); SCCI_UPDATE_FDR(1); SCCI_UPDATE_VOLUME(1); SCCI_UPDATE_MASK(1);
  SCCI_UPDATE_WAVEFORM(2); SCCI_UPDATE_FDR(2); SCCI_UPDATE_VOLUME(2); SCCI_UPDATE_MASK(2);
  SCCI_UPDATE_WAVEFORM(3); SCCI_UPDATE_FDR(3); SCCI_UPDATE_VOLUME(3); SCCI_UPDATE_MASK(3);
  SCCI_UPDATE_WAVEFORM(4); SCCI_UPDATE_FDR(4); SCCI_UPDATE_VOLUME(4); SCCI_UPDATE_MASK(4);
  scc_wave_buffer.updated = 0;
}

void SCC_play(struct SCC * scc) {
  if (!scc->slot) return;
  const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
  msx_ENASLT(scc->slot, (void *)PAGE_ADDR(2));
  if (scc->mode == 1) {
    SCC_update_registers();
  }
  else if (scc->mode == 2) {
    SCCI_update_registers();
  }
  msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
}

void SCC_set_waveform(uint8_t ch, const int8_t waveform[32]) {
  assert(ch < 5);
  memcpy(&scc_wave_buffer.waveform[ch], waveform, 32);
  scc_wave_buffer.updated |= 1 << ch;
}
