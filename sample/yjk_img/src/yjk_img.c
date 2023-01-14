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
 * \file yjk_img.c
 */

#include <msx.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

#include <bmem.h>
#include <resources.h>

void main(void) {
  screen12();
  border_color(0);

  vdp_cmd_execute_HMMV(0, 212, 256, 44, 0);
  vdp_set_hscroll_mask(true);

  resource_bload_s("KOTATSU2.S12");

  // foreground: (Y,J,K) = (31,-1,-1)
  // background: (Y,J,K) = ( 7,-1,-1)
  text_color(0xff, 0x3f);
  locate(7, 23);
  print("Twitter @mori0091");
  locate(3, 24);
  print("github.com/mori0091/libmsx");

  // waveform for raster scroll
  const int16_t wave[] = {
    0, 3, 6, 7, 8, 7, 6, 3,
    0,-3,-6,-7,-8,-7,-6,-3,
  };

  for (;;) {
    uint16_t idx = 0;
    uint8_t i = 0;

    // ---- waving (raster scroll) ----
    uint16_t t = JIFFY;
    while (JIFFY - t < 300) {
      __asm__("di");
      while (!(VDP_GET_STATUS_REGISTER_VALUE() & 0x80))
        ;
      JIFFY++;
      uint16_t y = 95 * 60 / msx_get_vsync_frequency();
      while (y--) {
        vdp_set_hscroll(wave[i]);
        i++;
        i &= 15;
      }
      idx++;
      i = idx & 15;
      __asm__("ei");
    }

    // ---- still image ----
    await_vsync();
    vdp_set_hscroll(0);
    sleep_ticks(60);

    // ---- shaking ----
    t = JIFFY;
    while (JIFFY - t < 120) {
      await_vsync();
      vdp_set_hscroll(wave[i]);
      i++;
      i &= 15;
    }

    // ---- still image ----
    await_vsync();
    vdp_set_hscroll(0);
    sleep_ticks(60);

    // ---- hopping ----
    int8_t y = -18;
    int8_t vy = -8;
    for (uint16_t x = 256; x--; ) {
      await_vsync();
      vdp_set_hscroll(x & 255);
      vdp_set_vscroll(y);
      y -= vy;
      vy++;
      if (8 < vy) {
        vy = -8;
      }
    }

    // ---- still image ----
    await_vsync();
    vdp_set_hscroll(0);
    vdp_set_vscroll(0);
    sleep_ticks(300);
  }
}
