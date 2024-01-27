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
 * \file opll_device.c
 */

#include "opll.h"

#include "config.h"
#include <stdint.h>

static volatile __sfr __at (0x7c) opll_port0;
static volatile __sfr __at (0x7d) opll_port1;

// `MSXturboR` 16bits free run timer, countup every 3.911us.
static volatile __sfr __at (0xe6) system_timer_lo_port;
static volatile __sfr __at (0xe7) system_timer_hi_port;

static void OPLL_write(uint8_t reg, uint8_t value) {
  __asm__("di");
  opll_port0 = reg;
  opll_port1 = value;
  // wait for 23.52us at least
  __asm__("ex (sp), hl");       // 4.75us @ 4MHz
  __asm__("ex (sp), hl");       // 4.75us @ 4MHz

  __asm__("ex (sp), hl");       // 4.75us @ 4MHz
  __asm__("ex (sp), hl");       // 4.75us @ 4MHz

  __asm__("ex (sp), hl");       // 4.75us @ 4MHz
  __asm__("ex (sp), hl");       // 4.75us @ 4MHz
}

static void OPLL_write_turboR(uint8_t reg, uint8_t value) {
  __asm__("di");
  opll_port0 = reg;
  opll_port1 = value;
  // wait for 23.52us at least
  const uint8_t t0 = system_timer_lo_port;
  for (; (uint8_t)(system_timer_lo_port - t0) < 7; )
    ;
}

const struct OPLL_Device OPLL_device = {
  .write = OPLL_write,
};

const struct OPLL_Device OPLL_device_turboR = {
  .write = OPLL_write_turboR,
};
