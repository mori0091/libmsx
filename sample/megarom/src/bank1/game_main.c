// -*- coding: utf-8-unix -*-
/**
 * \file game_main.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma codeseg BANK1

#include <msx.h>

#include "game_main.h"

#include "macros.h"
#include "sound_data.h"
#include "interrupt_handler.h"

_Noreturn
static void main_loop(void) {
  uint16_t t = 0;
  vdp_set_visible(true);
  vdp_set_sprite_visible(true);
  for (;;) {
    t++;
    await_vsync();              // wait for next VSYNC
    set_hscroll(t & 0x1ff);     // see interrupt_handler.c
    if (t % 170 == 0) {
      sound_effect(&se_1);      // play sound effects
    }
  }
}

void game_main(void) __banked {
  // Initialize the sound driver.
  // This must be called once at least, before the sound_player() is called.
  sound_init();

  // setup user-defined interrupt handler
  // - sample/megarom/include/interrupt_handler.h
  // - sample/megarom/src/code/interrupt_handler.c
  setup_interrupt();

  sound_set_repeat(true);       // Turn on the auto-repeat of BGM.
  sound_set_bgm(&bgm_1);        // Register the BGM to be played.
  sound_start();                // Start the BGM.

  main_loop();
}
