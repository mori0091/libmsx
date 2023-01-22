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
 * \file driving.c
 * \brief `MSX2+` driving game demo.
 *
 * Proof of concept of the following technique.
 * - HSYNC interrupt
 *   - Screen division
 *   - Raster scroll
 * - Color palette animation
 * - Screen vibration
 * - Sprites
 */

#include <msx.h>
#include <screen.h>
#include <text.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "./sprites.h"

#define LINES          (212)    // Height of the screen

#define HSYNC_LINE0    (20)     // Top line of the drawing area for distant landscape
#define HORIZON_LINE   (119)    // Top line of the road / Line of the horizon.

#define RASTER_LINES   (212 - HORIZON_LINE) // must be multiple of three.
#define RASTER_ROWS    (RASTER_LINES / 3)

#define CENTER         (128)    // scroll amount of the +/- zero position

#define MAX_R          (85)     // maximum curvature (8x)
#define MAX_POS_X      (MAX_R * 10) // upper limit of the vehicle's lateral position (8x)

static bool start_raster;
static uint8_t idx;
static uint16_t scroll_x[2][RASTER_ROWS];
static uint16_t * p_scroll_x;
static uint16_t bg_scroll_x;
static uint16_t bg_scroll_x_d;

static int8_t R;                // target curvature (8x)
static int8_t r;                // the lateral road curvature at the horizon (8x)
static int8_t r0;               // the lateral road curvature at vehicle position (8x)
static int8_t curvatures[RASTER_ROWS]; // ring buffer of lateral road curvatures
static uint8_t curvatures_idx;

static int8_t vx;               // vehicle's lateral speed
static int16_t x_pos;           // vehicle's lateral position (integer part)
static int16_t x_pos_d;         // vehicle's lateral position (decimal part)

// inline void on_vsync(void) {
//   vdp_set_hscroll(0);
//   VDP_SET_CONTROL_REGISTER(19, (RG19SA = HSYNC_LINE0 - 6));
//   start_raster = false;
// }

// inline void on_hsync(void) {
//   if (!start_raster) {
//     vdp_set_hscroll(bg_scroll_x);
//     VDP_SET_CONTROL_REGISTER(19, (RG19SA = HORIZON_LINE - 6));
//     start_raster = true;
//     return;
//   }
//   p_scroll_x = &scroll_x[idx][0];
//   VDP_SET_STATUS_REGISTER_POINTER(2);
//   for (uint8_t n = RASTER_ROWS; n--; ) {
//     while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
//       ;
//     vdp_set_hscroll(*p_scroll_x++);
//     while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
//       ;
//   }
// }

inline uint8_t hscroll_register_r26_value_from(uint16_t x) {
  return ((x + 7) >> 3) & 0x3f;
}

inline uint8_t hscroll_register_r27_value_from(uint16_t x) {
  return (-x & 7);
}

inline void on_vsync(void) {
  VDP_SET_CONTROL_REGISTER(26, 0);
  VDP_SET_CONTROL_REGISTER(27, 0);
  VDP_SET_CONTROL_REGISTER(19, (RG19SA = HSYNC_LINE0 - 6));
  start_raster = false;
}

inline void on_hsync(void) {
  if (!start_raster) {
    // scroll the distant landscape area
    VDP_SET_CONTROL_REGISTER(26, hscroll_register_r26_value_from(bg_scroll_x));
    VDP_SET_CONTROL_REGISTER(27, hscroll_register_r27_value_from(bg_scroll_x));
    // setup HSYNC interrupt line to HORIZON_LINE (top of the ground area)
    VDP_SET_CONTROL_REGISTER(19, (RG19SA = HORIZON_LINE - 4));
    start_raster = true;
    return;
  }
  // scroll the ground area every 3 lines (raster scroll)
  const uint8_t * p = (const uint8_t *)&scroll_x[idx][0];
  VDP_SET_STATUS_REGISTER_POINTER(2);
  for (uint8_t n = RASTER_ROWS; n--; ) {
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    VDP_SET_CONTROL_REGISTER(26, *p++);
    VDP_SET_CONTROL_REGISTER(27, *p++);
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
  }
}

void interrupt_handler(void) {
  VDP_SET_STATUS_REGISTER_POINTER(1);
  if (VDP_GET_STATUS_REGISTER_VALUE() & 1) {
    on_hsync();
    VDP_SET_STATUS_REGISTER_POINTER(0);
    if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
      JIFFY++;
      vsync_busy = false;
    }
    __asm__("ei");
    return;
  }
  VDP_SET_STATUS_REGISTER_POINTER(0);
  if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
    on_vsync();
    JIFFY++;
    vsync_busy = false;
  }
  __asm__("ei");
}

static void draw_graphics(void) {
  // Clear page 0 and 1
  vdp_cmd_execute_HMMV(0,   0, 256, 212, 0);
  vdp_cmd_execute_HMMV(0, 256, 256, 212, 0);

  // Draw landscape (over the horizon)
  {
    uint8_t c = 1;
    for (uint16_t j = 0; j < 256; j += 8) {
      // left half (page 0)
      vdp_cmd_execute_HMMV(j,   0, 8, HORIZON_LINE, (c << 4) | (c & 15));
      // right half (page 1)
      vdp_cmd_execute_HMMV(j, 256, 8, HORIZON_LINE, (c << 4) | (c & 15));
      c++;
      if (c > 11) {
        c = 1;
      }
    }
  }

  // Draw border lines of road
  for (uint8_t k = 0; k < 8; ++k) {
    // left half (page 0)
    vdp_cmd_execute_LINE(255, HORIZON_LINE,      64-4+k, 211    , 14, VDP_CMD_IMP);
    vdp_cmd_execute_LINE(255, HORIZON_LINE,     192-4+k, 211    , 14, VDP_CMD_IMP);
    // right half (page 1)
    vdp_cmd_execute_LINE(  0, HORIZON_LINE+256, 192-4+k, 211+256, 14, VDP_CMD_IMP);
    vdp_cmd_execute_LINE(  0, HORIZON_LINE+256,  64-4+k, 211+256, 14, VDP_CMD_IMP);
  }

  // Draw the ground
  {
    uint8_t c = 0xcc;
    uint8_t h = 4;
    for (uint8_t i = HORIZON_LINE; i < 212; ) {
      uint8_t hh = h >> 2;
      // left half (page 0)
      vdp_cmd_execute_LMMV(0, i,     256, hh, c, VDP_CMD_OR);
      // right half (page 1)
      vdp_cmd_execute_LMMV(0, i+256, 256, hh, c, VDP_CMD_OR);
      i += hh;
      c ^= 0x11;
      h++;
    }
  }

  locate(2, 0); print("ARROW KEY: Steering control");
  locate(4, 1); print("MSX2+ Raster scroll demo");
}

static void init_variables(void) {
  R = r = 0;
  x_pos = x_pos_d = vx = 0;

  idx = 0;
  // for (uint8_t n = RASTER_ROWS; n--; ) {
  //   scroll_x[0][n] = CENTER;
  //   scroll_x[1][n] = CENTER;
  // }
  const uint8_t r26 = hscroll_register_r26_value_from(CENTER); // R#26
  const uint8_t r27 = hscroll_register_r27_value_from(CENTER); // R#27
  uint8_t * p = (uint8_t *)scroll_x;
  for (uint8_t n = 2 * RASTER_ROWS; n--; ) {
    *p++ = r26;
    *p++ = r27;
  }

  memset(curvatures, 0, sizeof(curvatures));
  curvatures_idx = 0;
}

static void road_animation(void) {
  if (JIFFY & 2) {
    vdp_set_palette(12, RGB(2, 2, 5));
    vdp_set_palette(13, RGB(1, 1, 4));
    vdp_set_palette(14, RGB(0, 0, 0));
    vdp_set_palette(15, RGB(7, 7, 7));
  }
  else {
    vdp_set_palette(12, RGB(1, 1, 4));
    vdp_set_palette(13, RGB(2, 2, 5));
    vdp_set_palette(14, RGB(7, 7, 7));
    vdp_set_palette(15, RGB(0, 0, 0));
  }
}

static void update_road_geometry(void) {
  // generate curve every 128 ticks
  if (!(JIFFY & 127)) {
    uint16_t a = rand();
    if (!(a & 3)) {
      // return to straight course in 25% probability.
      R = 0;
      hide_curve_indicator();
    }
    else {
      a = (a >> 2) & 255;
      if (a < 2 * MAX_R + 1) {
        // change target curvature `R` to random value in range of -MAX_R to +MAX_R
        R = a - MAX_R;
        // show curve sign before reach to the curve if it is sharp curve.
        if (MAX_R / 8 < R) {
          show_curve_indicator_right();
        }
        else if (R < -MAX_R / 8) {
          show_curve_indicator_left();
        }
        else if (-MAX_R / 16 < R && R < MAX_R / 16) {
          hide_curve_indicator();
        }
      }
      else {
        // keep target curvature `R`
      }
    }
  }
  // Gradually bring the curvature closer to the target value.
  if (r < R) {
    r++;
  }
  if (R < r) {
    r--;
  }
  // update the ring buffer (history) of curvature
  curvatures[curvatures_idx++] = r;
  if (sizeof(curvatures) <= curvatures_idx) {
    curvatures_idx = 0;
  }
  r0 = curvatures[curvatures_idx];
}

inline void update_raster_scroll_table(void) {
  const uint8_t k = idx ^ 1;
  uint16_t x = (RASTER_ROWS - 1) * x_pos;
  uint16_t dx = 0;
  const int8_t * p_curvatures = &curvatures[curvatures_idx];
  uint8_t j = curvatures_idx;
  uint8_t * p = ((uint8_t *)&scroll_x[k][RASTER_ROWS]) - 1;
  for (uint8_t i = RASTER_ROWS; i--; ) {
    // scroll_x[k][i] = (CENTER + (x >> 7)) & 511;
    const uint16_t sx = (CENTER + (x >> 7)) & 511;
    *p-- = hscroll_register_r27_value_from(sx); // R#27
    *p-- = hscroll_register_r26_value_from(sx); // R#26
    dx -= *p_curvatures++;
    x += dx - x_pos;
    j++;
    if (sizeof(curvatures) <= j) {
      j = 0;
      p_curvatures = &curvatures[0];
    }
  }
  idx ^= 1;
}

void main(void) {
  screen5();
  color(11, 0, 0);
  cls();

  setup_sprites();
  draw_graphics();

  vdp_set_image_table(0x08000ULL); // SET PAGE 1 (Odd page must be selected for dual page horizontal-scroll)
  vdp_set_hscroll_dual_page(true); // Enable dual page horizontal-scroll
  vdp_set_hscroll_mask(true);      // Hide (ugly) left most 8-pixels
  vdp_set_adjust(-4, 0);           // Centering the display area

  // setup HSYNC / VSYNC interrupt handler
  vdp_set_control(19, (RG19SA = HSYNC_LINE0 - 6)); // set line of HSYNC interrupt
  vdp_set_control(0, (RG0SAV |= 0x10));            // enable HSYNC interrupt
  set_interrupt_handler(interrupt_handler);

  init_variables();

  // main loop of the game
  for (;;) {
    await_vsync();
    road_animation();           // palette animation of the road.
    update_road_geometry();     // update curvature of the road.

    // update scroll amount for distant landscape
    bg_scroll_x_d -= r0;
    bg_scroll_x -= bg_scroll_x_d / 16;
    bg_scroll_x_d %= 16;

    // steering
    uint8_t joy = joypad_get_state(0);
    if (joy & VK_RIGHT) {
      vx += 8;
    }
    else if (joy & VK_LEFT) {
      vx -= 8;
    }
    else {
      if (0 < vx) {
        vx -= 4;
      }
      else if (vx < 0) {
        vx += 4;
      }
    }
    if (64 < vx) {
      vx = 64;
    }
    else if (vx < -64) {
      vx = -64;
    }
    // slide vehicle's lateral position according to steering and road curvature.
    x_pos_d += vx;
    x_pos_d -= 2 * r0;
    x_pos += x_pos_d / 8;
    x_pos_d %= 8;

    if (MAX_POS_X < x_pos) {
      x_pos = MAX_POS_X;
      border_color(8);          // Warning! sliding off the way!
      vdp_set_adjust(-4, (rand() & 7) - 4); // vibration
    }
    else if (x_pos < -MAX_POS_X) {
      x_pos = -MAX_POS_X;
      border_color(8);          // Warning! sliding off the way!
      vdp_set_adjust(-4, (rand() & 7) - 4); // vibration
    }
    else {
      border_color(0);
      vdp_set_adjust(-4, 0);
    }

    // update scroll amount for each raster
    update_raster_scroll_table();
  }
}
