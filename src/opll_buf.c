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

#include <opll_buf.h>

#include <stdint.h>
#include <string.h>

uint8_t opll_buffer[64];

static struct {
  uint8_t len;
  uint8_t buf[64];
} fifo;

void OPLL_init(void) {
  memset(opll_buffer, 0, sizeof(opll_buffer));
  memset(&fifo, 0, sizeof(fifo));
}

void OPLL_put(uint8_t reg, uint8_t val) {
  if (OPLL_get(reg) == val) return;
  OPLL_set(reg, val);
  if (fifo.len < sizeof(fifo.buf)) {
    fifo.buf[fifo.len++] = reg;
  }
}

void OPLL_stop(struct OPLL * opll) {
  if (!opll || !opll->slot) return;
  __asm__("di");
  void (*write)(uint8_t, uint8_t) = opll->device->write;
  for (uint8_t reg = 0x20; reg <= 0x28; reg++) {
    // SUS-OFF, KEY-OFF for all channels
    write(reg, OPLL_get(reg) & 0x0f);
  }
}

void OPLL_play(struct OPLL * opll) {
  if (!opll || !opll->slot || !fifo.len) return;
  __asm__("di");
  void (*write)(uint8_t, uint8_t) = opll->device->write;
  while (fifo.len) {
    uint8_t reg = fifo.buf[--fifo.len];
    write(reg, OPLL_get(reg));
  }
}
