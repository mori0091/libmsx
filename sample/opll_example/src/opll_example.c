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
 * \file opll_example.c
 *
 * \brief
 * Detects the MSX-MUSIC (OPLL) and plays a test tone if detected.
 */

#include <string.h>

#include <msx.h>
#include <opll.h>

static struct OPLL opll;

#define LO_BYTE(x) (uint8_t)((x) & 0xff)
#define HI_BYTE(x) (uint8_t)(((x) >> 8) & 0xff)

#define SUS_OFF  (0)
#define KEY_OFF  (0)
#define SUS_ON   (uint16_t)(1 << 13)
#define KEY_ON   (uint16_t)(1 << 12)
#define BLOCK(x) (uint16_t)((x) << 9)
#define F_NUM(x) (uint16_t)(x)

#define INST(x)  (uint8_t)((x) << 4)
#define VOL(x)   (uint8_t)((x) & 0x0f)

void test_OPLL(void) {
  // If OPLL was not found, nothing is done.
  if (!opll.slot) return;
  {
    // 9 channels mode
    opll.device->write(0x0e, 0x00);

    // ch1 : Forces to KEY-OFF before next KEY-ON.
    opll.device->write(0x10, 0);
    opll.device->write(0x20, 0);

    // ch1 : INST=3, VOL=0
    // \note
    // VOL(0) means maximum volume, VOL(15) means minimum volume.
    // VOL(a) means "-3a dB of the maximum volume" (i.e. Vmax * 10^(-0.3a))
    opll.device->write(0x30, (INST(3) | VOL(0)));

    // ch1 : O4 G
    uint16_t x = (SUS_OFF | KEY_ON | BLOCK(4) | F_NUM(257));
    opll.device->write(0x10, LO_BYTE(x));
    opll.device->write(0x20, HI_BYTE(x));

    // \note
    // Interrupts are disabled by calling `opll.device->write()`, so interrupts
    // must be enabled at the end.
    __asm__("ei");
    sleep_millis(2000);
  }
}

void main(void) {
  OPLL_find(&opll);             // Find OPLL
  OPLL_enable(&opll);           // Enable OPLL, if found.
  for (;;) {
    test_OPLL();
  }
}
