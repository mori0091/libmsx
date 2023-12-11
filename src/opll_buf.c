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
 * \file opll_buf.c
 */

#include "opll_buf.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

static uint8_t registers[64];
static struct {
  uint8_t len;
  uint8_t buf[64];
} fifo;

void OPLL_init(void) {
  memset(registers, 0, sizeof(registers));
  memset(&fifo, 0, sizeof(fifo));
}

void OPLL_put(uint8_t reg, uint8_t val) {
  assert(reg <= 0x07 ||
         reg == 0x0e ||
         reg == 0x0f ||
         (0x10 <= reg && reg <= 0x18) ||
         (0x20 <= reg && reg <= 0x28) ||
         (0x30 <= reg && reg <= 0x38));
  registers[reg] = val;
  if (sizeof(fifo.buf) <= fifo.len) {
    // buffer full
    return;
  }
  fifo.buf[fifo.len++] = reg;
}

void OPLL_stop(struct OPLL * opll) {
  if (!opll || !opll->slot) return;
  __asm__("di");
  for (uint8_t reg = 0x20; reg <= 0x28; reg++) {
    // SUS-OFF, KEY-OFF for all channels
    opll->device->write(reg, registers[reg] & 0x0f);
  }
}

void OPLL_play(struct OPLL * opll) {
  if (!opll || !opll->slot) return;
  const uint8_t * p = fifo.buf;
  __asm__("di");
  for (uint8_t n = fifo.len; n--; ) {
    uint8_t reg = *p++;
    opll->device->write(reg, registers[reg]);
  }
  fifo.len = 0;
}
