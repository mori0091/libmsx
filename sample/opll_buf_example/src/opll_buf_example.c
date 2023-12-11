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
 * \file opll_buf_example.c
 *
 * \brief
 * OPLL Buffer example.
 *
 * This example shows a sound driver/replayer template for the OPLL sound chip.
 */

#include <msx.h>
#include <opll_buf.h>

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


static struct OPLL opll;
static bool paused;

// Initialize the replayer.
void init(void) {
  if (OPLL_find(&opll)) {
    // Enable OPLL.
    OPLL_enable(&opll);
    // Initialize internal buffer.
    OPLL_init();
    // 9 channels mode
    OPLL_put(0x0e, 0x00);
  }
}

// Main routine of the replayer.
// \note This shall be called every VSYNC timing from interrupt handler.
void play(void) {
  if (!opll.slot) return;
  if (paused) return;

  // Apply buffered values to the OPLL registers.
  OPLL_play(&opll);

  // Decode one frame of music data and set them to buffers.
  // (Rewrite this block for developping yourown sound driver / replayer)
  {
    static uint8_t t = 0;

    // One beat period at 120 bpm.
    const uint8_t len = msx_get_vsync_frequency() * 60 / 120;

    if (!t) {
      // ch1 : O4 G, INST=3, VOL=0
      // \note
      // VOL(0) means maximum volume, VOL(15) means minimum volume.
      // VOL(a) means "-3a dB of the maximum volume" (i.e. Vmax * 10^(-0.3a))
      uint16_t x = (SUS_OFF | KEY_ON | BLOCK(4) | F_NUM(257));
      OPLL_put(0x30, (INST(3) | VOL(0)));
      OPLL_put(0x20, HI_BYTE(x));
      OPLL_put(0x10, LO_BYTE(x));
    }
    else if (t == len * 4/5) {
      // ch1: KEY-OFF
      uint16_t x = (SUS_OFF | KEY_OFF | BLOCK(4) | F_NUM(257));
      // OPLL_put(0x30, (INST(3) | VOL(0)));
      OPLL_put(0x20, HI_BYTE(x));
      // OPLL_put(0x10, LO_BYTE(x));
    }

    t++;
    if (t == len) {
      t = 0;
    }
  }
}

// Start / Resume the replayer.
void start(void) {
  paused = false;
}

// Pause the replayer.
void pause(void) {
  paused = true;
  OPLL_stop(&opll);
  __asm__("ei");
}

// Return whether the replayer is paused or not.
bool is_paused(void) {
  return paused;
}

// Stop the replayer.
void stop(void) {
  pause();
  // Re-initialize internal buffer.
  // (if needed)
  OPLL_init();
}

void main(void) {
  // Initialize our replayer.
  init();
  // Set our replayer `play()` to VSYNC handler
  set_vsync_handler(play);
  // Start our replayer.
  start();

  uint8_t pressed;
  for (;;) {
    await_vsync();
    uint8_t joy = joypad_get_state(0);
    uint8_t clicked = pressed & ~joy;
    pressed = joy;
    if (clicked & VK_FIRE_0) {
      if (is_paused()) {
        start();
      }
      else {
        pause();
      }
    }
  }
}
