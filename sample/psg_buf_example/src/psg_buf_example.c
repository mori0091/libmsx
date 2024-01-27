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
 * \file psg_buf_example.c
 *
 * \brief
 * PSG Buffer example.
 *
 * This example shows a sound driver/replayer template for the PSG sound chip.
 */

#include <msx.h>
#include <ay_3_8910.h>

#define LO_BYTE(x) (uint8_t)((x) & 0xff)
#define HI_BYTE(x) (uint8_t)(((x) >> 8) & 0xff)

static bool paused;

// Initialize the replayer.
void init(void) {
  ay_3_8910_init();
}

// Main routine of the replayer.
// \note This shall be called every VSYNC timing from interrupt handler.
void play(void) {
  if (paused) return;

  // Apply buffered values to the PSG registers.
  ay_3_8910_play();

  // Decode one frame of music data and set them to buffers.
  // (Rewrite this block for developping yourown sound driver / replayer)
  {
    // Update buffered values for each channels (if needed).
    for (uint8_t ch = 0; ch < 3; ch++) {
      // Update frequency division ratio (i.e., period)
      ay_3_8910_buffer[2*ch+0] = LO_BYTE(0x11d); // O4 G (lo-byte)
      ay_3_8910_buffer[2*ch+1] = HI_BYTE(0x11d); // O4 G (hi-byte)

      // Update volume.
      ay_3_8910_buffer[ch+8] = 8;
    }
    // Update buffered mixer value (if needed).
    ay_3_8910_buffer[7] = 0xb8; // enable TONE of all 3 channels
    // ...and so on
  }
}

// Start / Resume the replayer.
void start(void) {
  paused = false;
}

// Pause the replayer.
void pause(void) {
  paused = true;
  ay_3_8910_stop();
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
  ay_3_8910_init();
}

void main(void) {
  // Initialize our replayer.
  init();
  // Set our replayer `play()` to VSYNC handler
  set_vsync_handler(play);
  // Start our replayer.
  start();

  uint8_t pressed = false;
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
