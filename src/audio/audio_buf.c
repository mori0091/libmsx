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
 * \file audio_buf.c
 */

#include <audio_buf.h>
#include <bios.h>
#include <io.h>

#include <string.h>

#include "audio__internal.h"
#include "../scc_io.h"

#define CMD_SCC_MIN      ((uint8_t)0x00) // corresponds to SCC/SCC+ base address of wavaforms
#define CMD_SCC_MAX      ((uint8_t)0xaf) // corresponds to SCC/SCC+ base address of channel-mask
#define CMD_PSG_MIN      ((uint8_t)0xb0) // corresponds to PSG R#0
#define CMD_PSG_MAX      ((uint8_t)0xbd) // corresponds to PSG R#13
#define CMD_OPLL_MIN     ((uint8_t)0xc0) // corresponds to OPLL $00
#define CMD_OPLL_MAX     ((uint8_t)0xf8) // corresponds to OPLL $38

#define CMD_SCC_WAVEFORM ((uint8_t)0xfa) // copy waveform from audio_buf_cache to SCC/SCC+
#define CMD_MAX          (255)

#define CMD_SCC(reg)     ((uint8_t)(CMD_SCC_MIN + (reg)))
#define CMD_PSG(reg)     ((uint8_t)(CMD_PSG_MIN + (reg)))
#define CMD_OPLL(reg)    ((uint8_t)(CMD_OPLL_MIN + (reg)))

#define CAPACITY (128)          // < 256

struct Request {
  uint8_t priority;
  uint8_t cmd;
  uint8_t val;
};
static struct FIFO {
  uint8_t len;
  struct Request buf[CAPACITY];
} fifo;

static uint8_t latest_priority[CMD_MAX+1];
uint8_t audio_buf_cache[CMD_MAX+1];

static void null_write(uint8_t reg, uint8_t val) {
  (void)reg;
  (void)val;
}
static void null_write_waveform(uint8_t ch) {
  (void)ch;
}

#define write_psg(reg, val)                     \
  psg_port0 = (reg);                            \
  psg_port1 = (val)

static void (* write_scc_waveform)(uint8_t ch);
static void (* write_scc)(uint8_t reg, uint8_t val);
static void (* write_opll)(uint8_t reg, uint8_t val);

static void write_scc_1_waveform(uint8_t ch) {
  if (4 <= ch) return;
  memcpy((void *)SCC_waveform[ch], audio_buf_cache + ch * 32, 32);
  SCC_channel_mask
    = (SCC_channel_mask & ~(1 << (ch)))
    | (audio_buf_cache[0xaf] & (1 << (ch)));
}

static void write_scc_1(uint8_t reg, uint8_t val) {
  if (0xb0 <= reg) {
    return;
  }
  else if (0xa0 <= reg) {
    ((uint8_t *)SCC_fdr)[reg - 0xa0] = val;
  }
  else if (reg < 0x80) {
    ((uint8_t *)SCC_waveform)[reg] = val;
  }
}

static void write_scc_2_waveform(uint8_t ch) {
  if (5 <= ch) return;
  memcpy((void *)SCCPlus_waveform[ch], audio_buf_cache + ch * 32, 32);
  SCCPlus_channel_mask
    = (SCCPlus_channel_mask & ~(1 << (ch)))
    | (audio_buf_cache[0xaf] & (1 << (ch)));
}

static void write_scc_2(uint8_t reg, uint8_t val) {
  if (0xb0 <= reg) {
    return;
  }
  else {
    ((uint8_t *)SCCPlus_waveform)[reg] = val;
  }
}

static void stop_psg(void) {
  audio_buf_cache[0xb8] = 0;
  audio_buf_cache[0xb9] = 0;
  audio_buf_cache[0xba] = 0;
  write_psg(8, 0);
  write_psg(9, 0);
  write_psg(10, 0);
}

static void stop_scc(void) {
  audio_buf_cache[0xaa] = 0;
  audio_buf_cache[0xab] = 0;
  audio_buf_cache[0xac] = 0;
  audio_buf_cache[0xad] = 0;
  audio_buf_cache[0xae] = 0;
  audio_buf_cache[0xaf] = 0;    // Channel mask
  audio_buf_cache[0xfa] = 0;    // Channel number of the last waveform change
  if (audio_.scc.slot) {
    const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
    msx_ENASLT(audio_.scc.slot, (void *)PAGE_ADDR(2));
    if (audio_.scc.mode == 1) {
      memset((void *)SCC_volume, 0, 5);
    }
    else if (audio_.scc.mode == 2) {
      memset((void *)SCCPlus_volume, 0, 5);
    }
    msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
  }
}

static void stop_opll(void) {
  if (audio_.opll.slot) {
    write_opll = audio_.opll.device->write;
    for (uint8_t reg = 0x20; reg <= 0x28; ++reg) {
      // SUS-OFF, KEY-OFF for all channels
      write_opll(reg, audio_buf_cache[CMD_OPLL(reg)] & 0x0f);
    }
  }
}

static void clear_scc_waveform(void) {
  memset(audio_buf_cache, 0, 5 * 32);
  if (audio_.scc.slot) {
    const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
    msx_ENASLT(audio_.scc.slot, (void *)PAGE_ADDR(2));
    if (audio_.scc.mode == 1) {
      memset((void *)SCC_waveform, 0, 4 * 32);
    }
    else if (audio_.scc.mode == 2) {
      memset((void *)SCCPlus_waveform, 0, 5 * 32);
    }
    msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
  }
}

void audio_buf_init(void) {
  clear_scc_waveform();
  audio_buf_clear();
  audio_buf_stop();
}

void audio_buf_stop(void) {
  stop_psg();
  stop_scc();
  stop_opll();
}

void audio_buf_clear(void) {
  memset(latest_priority, 0, sizeof(latest_priority));
  fifo.len = 0;
}

void audio_buf_put(uint8_t priority, uint8_t cmd, uint8_t val) {
  if (CAPACITY <= fifo.len) return; // buffer full
  if (!priority) {
    audio_buf_cache[cmd] = val;
  }
  if (priority < latest_priority[cmd]) return;
  latest_priority[cmd] = priority;
  fifo.buf[fifo.len].priority = priority;
  fifo.buf[fifo.len].cmd = cmd;
  fifo.buf[fifo.len].val = val;
  fifo.len++;
}

void audio_buf_restore(void) {
  // SCC/SCC+
  latest_priority[0xfa] = 0;
  for (uint8_t cmd = 0xa0; cmd < 0xb0; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd]);
    }
  }
  // PSG
  // for (uint8_t cmd = 0xb0; cmd < 0xbe; ++cmd) {
  for (uint8_t cmd = 0xb0; cmd < 0xbd; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd]);
    }
  }
  latest_priority[0xbd] = 0;
  // OPLL $00..$07
  for (uint8_t cmd = 0xc0; cmd < 0xc8; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd]);
    }
  }
  // OPLL $0e
  {
    if (latest_priority[0xce]) {
      latest_priority[0xce] = 0;
      audio_buf_put(0, 0xce, audio_buf_cache[0xce] & 0x20);
    }
  }
  // OPLL $10..$18
  for (uint8_t cmd = 0xd0; cmd < 0xd9; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd]);
    }
  }
  // OPLL $20..$28
  for (uint8_t cmd = 0xe0; cmd < 0xe9; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd] & 0x2f);
    }
  }
  // OPLL $30..$38
  for (uint8_t cmd = 0xf0; cmd < 0xf9; ++cmd) {
    if (latest_priority[cmd]) {
      latest_priority[cmd] = 0;
      audio_buf_put(0, cmd, audio_buf_cache[cmd]);
    }
  }
}

static void write(uint8_t cmd, uint8_t val) {
  if (cmd <= CMD_SCC_MAX) {
    write_scc(cmd, val);
  }
  else if (/* CMD_PSG_MIN <= cmd && */ cmd <= CMD_PSG_MAX) {
    write_psg(cmd & 0x0f, val);
  }
  else if (CMD_OPLL_MIN <= cmd && cmd <= CMD_OPLL_MAX) {
    write_opll(cmd - CMD_OPLL_MIN, val);
  }
  else if (0xfa == cmd) {
    write_scc_waveform(val);
  }
}

void audio_buf__play(void) {
  uint8_t n = fifo.len;
  fifo.len = 0;
  struct Request * p = fifo.buf;
  while (n--) {
    uint8_t cmd = p->cmd;
    if (latest_priority[cmd] == p->priority) {
      write(cmd, p->val);
    }
    p++;
  }
}

void audio_buf_play(void) {
  if (audio_.scc.slot) {
    if (audio_.scc.mode == 2) {
      write_scc_waveform = write_scc_2_waveform;
      write_scc = write_scc_2;
    }
    else if (audio_.scc.mode == 1){
      write_scc_waveform = write_scc_1_waveform;
      write_scc = write_scc_1;
    }
    const uint8_t slot_p2 = msx_get_slot((void *)PAGE_ADDR(2));
    msx_ENASLT(audio_.scc.slot, (void *)PAGE_ADDR(2));
    audio_buf__play();
    msx_ENASLT(slot_p2, (void *)PAGE_ADDR(2));
  }
  else {
    write_scc_waveform = null_write_waveform;
    write_scc = null_write;
    audio_buf__play();
  }
}
