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

#include <assert.h>
#include <stdint.h>
#include <string.h>

#define ASSERT_REGISTER_IS_VALID(reg)        \
  assert((reg) <= 0x07 ||                    \
         (reg) == 0x0e ||                    \
         (reg) == 0x0f ||                    \
         (0x10 <= (reg) && (reg) <= 0x18) || \
         (0x20 <= (reg) && (reg) <= 0x28) || \
         (0x30 <= (reg) && (reg) <= 0x38))

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
  OPLL_set(reg, val);
  if (fifo.len < sizeof(fifo.buf)) {
    fifo.buf[fifo.len++] = reg;
  }
}

void OPLL_set(uint8_t reg, uint8_t val) {
  ASSERT_REGISTER_IS_VALID(reg);
  registers[reg] = val;
}

uint8_t OPLL_get(uint8_t reg) {
  ASSERT_REGISTER_IS_VALID(reg);
  return registers[reg];
}

void OPLL_stop(struct OPLL * opll) {
  if (!opll || !opll->slot) return;
  __asm__("di");
  void (*write)(uint8_t, uint8_t) = opll->device->write;
  for (uint8_t reg = 0x20; reg <= 0x28; reg++) {
    // SUS-OFF, KEY-OFF for all channels
    write(reg, registers[reg] & 0x0f);
  }
}

void OPLL_play(struct OPLL * opll) {
  if (!opll || !opll->slot || !fifo.len) return;
  __asm__("di");
  void (*write)(uint8_t, uint8_t) = opll->device->write;
  while (fifo.len) {
    uint8_t reg = fifo.buf[--fifo.len];
    write(reg, registers[reg]);
  }
}
