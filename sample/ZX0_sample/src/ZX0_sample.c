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
 * \file ZX0_sample.c
 */

#include "bios.h"
#include "slot.h"
#include "vdp.h"
#include <msx.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

#include <resources.h>
#include <ZX0_decompress.h>

static const char * MSX[] = {
  "MSX", "MSX2", "MSX2+", "MSXturboR",
};

static bool is_mirrored(uint8_t * a, uint8_t * b) {
  bool ret = false;
  if (*a == *b) {
    const uint8_t x = *a;
    *a = ~x;
    ret = (~x == *b);
    *a = x;
  }
  return ret;
}

static uint8_t msx1_main_RAM_size_in_K(void) {
  const uint8_t p3slot = msx_get_slot(PAGE_ADDR(3));
  if (slot_is_read_only(p3slot, PAGE_ADDR(2))) {
    return is_mirrored((uint8_t *)PAGE_ADDR(3),
                       (uint8_t *)PAGE_ADDR(3) + PAGE_SIZE / 2)
      ? 8
      : 16;
  }
  if (slot_is_read_only(p3slot, PAGE_ADDR(1))) {
    return 32;
  }
  assert(!slot_is_read_only(p3slot, PAGE_ADDR(0)));
  return 64;
}

static void print_MSX(void) {
  uint8_t ver = msx_get_version();
  if (ver < 4) {
    print(MSX[ver]);
    if (!ver) {
      print(" (RAM "); printu(msx1_main_RAM_size_in_K()); print("K)");
    }
  }
  else {
    print("??");
  }
}

static void description(const char * msg) {
  vdp_set_visible(false);
  cls();
  locate(0, 10);
  print(msg); print("\n");
  print_MSX(); print(msx_get_cpu_mode() ? " / R800\n" : " / Z80\n");
  print("VSYNC "); printu(msx_get_vsync_frequency()); print("Hz");
  locate(0, 0);
  vdp_set_visible(true);
  sleep_millis(1500);
}

static void print_time(uint16_t t) {
  printu(t);
  print("frames @");
  printu(msx_get_vsync_frequency());
  print("Hz");
}

// // MSX1 (TMS9918A VDP) colors simulated by MSX2 color palette.
// static const palette_t msx1_colors[16] = {
//   RGB(0,0,0), RGB(0,0,0), RGB(0,5,1), RGB(1,5,2),
//   RGB(1,1,6), RGB(2,2,7), RGB(5,1,1), RGB(1,6,7),
//   RGB(7,1,1), RGB(7,3,2), RGB(5,4,1), RGB(6,5,2),
//   RGB(0,4,1), RGB(5,1,4), RGB(5,5,5), RGB(7,7,7),
// };

// MSX1 (TMS9918A VDP) colors simulated by MSX2 color palette.
static const palette_t msx1_colors[16] = {
  RGB(0,0,0), RGB(0,0,0), RGB(0,5,1), RGB(1,5,2),
  RGB(1,1,6), RGB(2,2,7), RGB(4,2,1), RGB(1,6,7),
  RGB(6,1,1), RGB(6,3,2), RGB(5,4,1), RGB(6,5,2),
  RGB(0,4,1), RGB(5,1,4), RGB(5,5,5), RGB(7,7,7),
};

// MSX2/MSX2+ (V9938/V9958 VDP) default colors.
static const palette_t msx2_colors[16] = {
  RGB(0,0,0), RGB(0,0,0), RGB(1,6,1), RGB(3,7,3),
  RGB(1,1,7), RGB(2,3,7), RGB(5,1,1), RGB(2,6,7),
  RGB(7,1,1), RGB(7,3,3), RGB(6,6,1), RGB(6,6,4),
  RGB(1,4,1), RGB(6,2,5), RGB(5,5,5), RGB(7,7,7),
};

// Colors for "KOTATSU.SC5"
static const palette_t sc5_colors[16] = {
  RGB(0,0,0), RGB(1,1,1), RGB(2,1,0), RGB(1,2,2),
  RGB(2,2,1), RGB(3,2,1), RGB(3,3,2), RGB(2,3,3),
  RGB(3,4,4), RGB(4,4,3), RGB(3,5,6), RGB(5,5,5),
  RGB(5,6,6), RGB(6,6,6), RGB(5,7,7), RGB(6,7,7),
};

// Colors for "KOTATSU.SC7"
static const palette_t sc7_colors[16] = {
  RGB(0,0,0), RGB(1,1,1), RGB(2,1,0), RGB(1,2,2),
  RGB(2,2,1), RGB(3,2,1), RGB(3,3,2), RGB(2,3,3),
  RGB(4,4,3), RGB(2,4,5), RGB(4,4,4), RGB(5,5,4),
  RGB(5,5,5), RGB(4,5,6), RGB(6,6,6), RGB(6,7,7),
};

void main(void) {

  for (;;) {
    uint16_t t;

    if (msx_get_version()) {
      // reset color palette to the default values.
      vdp_write_palette(msx1_colors);
    }

    {
      vdp_set_visible(false);
      screen2();
      color(15, 0, 0);
      vdp_set_visible(true);

      description("SCREEN 2 / 256x192 2bpp\nRAW image");
      t = JIFFY;
      resource_bload_s("KOTATSU.SC2");
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 2 / 256x192 2bpp\nZX0 compressed image");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem("KOTATSU.SC2.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);
    }
    if (msx_get_version() || !slot_is_read_only(msx_get_slot(PAGE_ADDR(3)), PAGE_ADDR(0))) {
      // MSX1 (RAM 64KiB), MSX2 or later
      description("SCREEN 2 / 256x192 2bpp\nZX0 compressed image\n(via main RAM page #0 buffer)");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem_buffer("KOTATSU.SC2.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);
    }

    if (0 < msx_get_version()) {
      vdp_set_visible(false);
      screen5();
      color(15, 0, 0);
      vdp_write_palette(sc5_colors);
      vdp_set_visible(true);

      description("SCREEN 5 / 256x212 4bpp\nRAW image");
      t = JIFFY;
      resource_bload_s("KOTATSU.SC5");
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 5 / 256x212 4bpp\nZX0 compressed image");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem("KOTATSU.SC5.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 5 / 256x212 4bpp\nZX0 compressed image\n(via main RAM page #0 buffer)");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem_buffer("KOTATSU.SC5.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      vdp_set_visible(false);
      screen7();
      color(15, 0, 0);
      vdp_write_palette(sc7_colors);
      vdp_set_visible(true);
    }

    if (0 < msx_get_version()) {
      description("SCREEN 7 / 512x212 4bpp\nRAW image");
      t = JIFFY;
      resource_bload_s("KOTATSU.SC7");
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 7 / 512x212 4bpp\nZX0 compressed image");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem("KOTATSU.SC7.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 7 / 512x212 4bpp\nZX0 compressed image\n(via main RAM page #0 buffer)");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem_buffer("KOTATSU.SC7.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);
    }

    if (0 < msx_get_version()) {
      vdp_set_visible(false);
      screen8();
      color(255, 0, 0);
      vdp_set_visible(true);

      description("SCREEN 8 / 256x212 8bpp\nRAW image");
      t = JIFFY;
      resource_bload_s("KOTATSU.SC8");
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 8 / 256x212 8bpp\nZX0 compressed image");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem("KOTATSU.SC8.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 8 / 256x212 8bpp\nZX0 compressed image\n(via main RAM page #0 buffer)");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem_buffer("KOTATSU.SC8.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);
    }

    if (1 < msx_get_version()) {
      vdp_set_visible(false);
      screen12();
      color(248, 0, 0);
      vdp_set_visible(true);

      description("SCREEN 12 / 256x212 8bpp\nRAW image");
      t = JIFFY;
      resource_bload_s("KOTATSU.S12");
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 12 / 256x212 8bpp\nZX0 compressed image");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem("KOTATSU.S12.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);

      description("SCREEN 12 / 256x212 8bpp\nZX0 compressed image\n(via main RAM page #0 buffer)");
      t = JIFFY;
      ZX0_decompress_resource_to_vmem_buffer("KOTATSU.S12.zx0", 0x00000UL);
      t = JIFFY - t;
      print_time(t);
      sleep_millis(1500);
    }

    // toggle Z80 / R800 (if running on MSXturboR)
    msx_set_cpu_mode(msx_get_cpu_mode() ? 0x80 : 0x82);
  }
}
