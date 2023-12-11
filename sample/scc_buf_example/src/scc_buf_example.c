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
 * \file scc_buf_example.c
 *
 * \brief
 * SCC/SCC+ Buffer example.
 *
 * This example shows a sound driver/replayer template for the Konami SCC/SCC+
 * sound chip.
 */

#include <msx.h>
#include <scc_buf.h>
#include <scc_wav.h>

static struct SCC scc;
static bool paused;

// Initialize the replayer.
void init(void) {
  if (SCC_find(&scc)) {
    // Set to SCC+ mode if available. (optional)
    // The mode will be applied to the sound chip when SCC_enable() is called.
    SCC_set_mode(&scc, 2);

    // Enable SCC/SCC+.
    // This must be called at least once.
    // It must also be called when the mode is changed by SCC_set_mode().
    SCC_enable(&scc);

    // Initialize `scc_buffer` and internal waveform buffer.
    SCC_init();

    // Initialize waveform buffers (if needed).
    SCC_set_waveform(0, SCC_WAVEFORM_PULSE_1_8); // 1/8 pulse
    SCC_set_waveform(1, SCC_WAVEFORM_PULSE_1_4); // 1/4 pulse
    SCC_set_waveform(2, SCC_WAVEFORM_SQUARE);    // 1/2 pulse / square
    SCC_set_waveform(3, SCC_WAVEFORM_TRIANGLE);  // triangle
    SCC_set_waveform(4, SCC_WAVEFORM_TRIANGLE);  // (available for SCC+ mode only)
  }
}

// Main routine of the replayer.
// \note This shall be called every VSYNC timing from interrupt handler.
void play(void) {
  if (!scc.slot) return;
  if (paused) return;

  // Apply buffered values to the SCC/SCC+ registers.
  SCC_play(&scc);

  // Decode one frame of music data and set them to buffers.
  // (Rewrite this block for developping yourown sound driver / replayer)
  {
    // Update buffered values for each channels (if needed).
    for (uint8_t ch = 0; ch < 5; ch++) {
      // Update waveform buffer
      // SCC_set_waveform(ch, SCC_WAVEFORM_TRIANGLE);

      // Update frequency division ratio (i.e., period)
      scc_buffer.fdr[ch] = 0x11d; // O4 G

      // Update volume.
      scc_buffer.volume[ch] = 2;
    }
    // Update buffered channel mask value (if needed).
    scc_buffer.channel_mask = 0x1F; // enable all 5 channels
  }
}

// Start / Resume the replayer.
void start(void) {
  paused = false;
}

// Pause the replayer.
void pause(void) {
  paused = true;
  SCC_stop(&scc);
  __asm__("ei");
}

// Return whether the replayer is paused or not.
bool is_paused(void) {
  return paused;
}

// Stop the replayer.
void stop(void) {
  pause();
  // Re-initialize `scc_buffer` and internal waveform buffer.
  // (if needed)
  SCC_init();
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
