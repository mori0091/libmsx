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

#define HSYNC_LINE0    (20) // Top line of the drawing area for distant landscape
#define BOTTOM_MARGIN  (0)

// Top line of the road / Line of the horizon.
#define HORIZON_LINE   (LINES - RASTER_LINES - BOTTOM_MARGIN)
#define HORIZON_BASE   (HORIZON_LINE+8)

#define RASTER_ROWS    (RASTER_LINES / LINES_PER_ROW)
#define RASTER_LINES   (88)     // must be multiple of LINES_PER_ROW.
#define LINES_PER_ROW  (4)

#define CENTER             (128) // scroll amount of the +/- zero position
#define HALF_WIDTH_OF_ROAD (192)

// maximum lateral curvature
#define MAX_RX             ((192L << 7) * 2 / RASTER_ROWS / RASTER_ROWS)
#define MAX_RY             (( 32L << 8) * 2 / RASTER_ROWS / RASTER_ROWS)
#define MIN_RY             (-((32L << 8) * 2 / RASTER_ROWS / RASTER_ROWS))

// upper limit of the vehicle's lateral position
#define MAX_POS_X          (((HALF_WIDTH_OF_ROAD + 12) << 7) / RASTER_ROWS)

static uint16_t tick;           // tick count in the game

static bool start_raster; // flag used in HSYNC interrupt routine for switching its proccess
static uint8_t idx;       // current index of the `scroll_table` (0 or 1)
static struct scroll_table {
  uint8_t r23;         // vertical scroll amount
  uint8_t r26;         // horizontal scroll amount (towards left by 8 dot unit)
  uint8_t r27;         // horizontal scroll amount (towards right by 1 dot unit)
} scroll_table[2][RASTER_ROWS]; // double buffered scroll table

static uint16_t bg_scroll_x;    // scroll amount of landscape (integer part)
static uint16_t bg_scroll_x_d;  // scroll amount of landscape (decimal part)
static int8_t bg_scroll_y;      // scroll amount of landscape

struct curvature {
  int16_t x;                    // lateral road curvatre
  int16_t y;                    // longitudinal road curvature
  uint16_t x_sum;               // cumulative sum of lateral road curvature
  uint16_t y_sum;               // cumulative sum of longitudinal road curvature
};
static struct curvature R;     // target road curvature
static struct curvature r;     // the road curvature at the horizon
static struct curvature r0;    // the road curvature at vehicle position

static struct curvature curvatures[RASTER_ROWS]; // ring buffer of road curvatures
static uint8_t curvatures_idx;  // next element's index of the ring buffer

static int8_t vx;               // vehicle's lateral speed
static int16_t x_pos;           // vehicle's lateral position (integer part)
static int16_t x_pos_d;         // vehicle's lateral position (decimal part)

inline uint8_t hscroll_register_r26_value_from(uint16_t x) {
  return ((uint8_t)((x + 7) >> 3)) & 0x3f;
}

inline uint8_t hscroll_register_r27_value_from(uint16_t x) {
  return (-((uint8_t)x)) & 7;
}

inline void enable_hsync_interrupt(void) {
  VDP_SET_CONTROL_REGISTER(0, (RG0SAV |= 0x10));
}

inline void disable_hsync_interrupt(void) {
  VDP_SET_CONTROL_REGISTER(0, (RG0SAV &= ~0x10));
}

// \note same as `vdp_set_image_table(loc)` but optimal.
inline void on_vsync(void) {
  // select VRAM page #1
  // \note same as `vdp_set_image_table(0x08000LL)` but optimal.
  VDP_SET_CONTROL_REGISTER(2, RG2SAV = ((0x08000LL >> 10) & 0x7F) | 0x1F);
  // reset scroll registers
  VDP_SET_CONTROL_REGISTER(23, 0);
  VDP_SET_CONTROL_REGISTER(26, 0);
  VDP_SET_CONTROL_REGISTER(27, 0);
  // setup HSYNC interrupt line to HSYNC_LINE0 (top of the landspcape area)
  VDP_SET_CONTROL_REGISTER(19, (RG19SA = (uint8_t)(HSYNC_LINE0 - 6)));
  start_raster = false;
  enable_hsync_interrupt();
}

inline void on_hsync(void) {
  disable_hsync_interrupt();
  if (!start_raster) {
    // select VRAM page #3
    // \note same as `vdp_set_image_table(0x18000LL)` but optimal.
    VDP_SET_CONTROL_REGISTER(2, RG2SAV = ((0x18000LL >> 10) & 0x7F) | 0x1F);
    // scroll the distant landscape area
    VDP_SET_CONTROL_REGISTER(23, bg_scroll_y);
    VDP_SET_CONTROL_REGISTER(27, hscroll_register_r27_value_from(bg_scroll_x));
    VDP_SET_CONTROL_REGISTER(26, hscroll_register_r26_value_from(bg_scroll_x));
    // setup HSYNC interrupt line to HORIZON_LINE (top of the ground area)
    VDP_SET_CONTROL_REGISTER(19, (RG19SA = (uint8_t)(bg_scroll_y + HORIZON_LINE - 6)));
    start_raster = true;
    enable_hsync_interrupt();
    return;
  }
  // select VRAM page #1
  // \note same as `vdp_set_image_table(0x08000LL)` but optimal.
  VDP_SET_CONTROL_REGISTER(2, RG2SAV = ((0x08000LL >> 10) & 0x7F) | 0x1F);
  // scroll the ground area every LINES_PER_ROW lines (raster scroll)
  const uint8_t * p = (const uint8_t *)scroll_table[idx];
  VDP_SET_STATUS_REGISTER_POINTER(2);
  for (uint8_t n = RASTER_ROWS; n--; ) {
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    VDP_SET_CONTROL_REGISTER(23, *p++);
    VDP_SET_CONTROL_REGISTER(26, *p++);
    VDP_SET_CONTROL_REGISTER(27, *p++);
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
#if (1 < LINES_PER_ROW)
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
#if (2 < LINES_PER_ROW)
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
#if (3 < LINES_PER_ROW)
    while (!(VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
    while ((VDP_GET_STATUS_REGISTER_VALUE() & (1 << 5)))
      ;
#endif
#endif
#endif
  }
}

void interrupt_handler(void) {
  if (VDP_GET_STATUS_REGISTER_VALUE() & 0x80) {
    on_vsync();
    JIFFY++;
    vsync_busy = false;
  }
  VDP_SET_STATUS_REGISTER_POINTER(1);
  if (VDP_GET_STATUS_REGISTER_VALUE() & 1) {
    on_hsync();
  }
  VDP_SET_STATUS_REGISTER_POINTER(0);
  __asm__("ei");
}

static void draw_graphics(void) {
  // Clear page 0..3
  vdp_cmd_execute_HMMV(0,   0, 256, LINES, 0);
  vdp_cmd_execute_HMMV(0, 256, 256, LINES, 0);
  vdp_cmd_execute_HMMV(0, 512, 256, LINES, 0);
  vdp_cmd_execute_HMMV(0, 768, 256, LINES, 0);

  // Draw landscape (over the horizon)
  {
    vdp_cmd_execute_HMMV(0, 512, 256, LINES, 0x77);
    vdp_cmd_execute_HMMV(0, 768, 256, LINES, 0x77);
    uint8_t c = 1;
    for (uint16_t j = 0; j < 256; j += 8) {
      const uint8_t h = rand() % (HORIZON_LINE - HSYNC_LINE0 - 8) + 8;
      // left half (page 0)
      vdp_cmd_execute_HMMV(j, HORIZON_LINE - h + 512, 8, h, (c << 4) | (c & 15));
      // right half (page 1)
      vdp_cmd_execute_HMMV(j, HORIZON_LINE - h + 768, 8, h, (c << 4) | (c & 15));
      c++;
      if (c > 11) {
        c = 1;
      }
    }
    vdp_cmd_execute_HMMV(0, HORIZON_LINE + 512, 256, LINES - HORIZON_LINE, 0x22);
    vdp_cmd_execute_HMMV(0, HORIZON_LINE + 768, 256, LINES - HORIZON_LINE, 0x22);
  }

  // Draw border lines of road
  for (int8_t k = -3; k < 3; ++k) {
    // left half (page 0)
    vdp_cmd_execute_LINE(255, HORIZON_BASE,     (HALF_WIDTH_OF_ROAD/3)+k, (LINES-BOTTOM_MARGIN-1)    , 14, VDP_CMD_IMP);
    vdp_cmd_execute_LINE(255, HORIZON_BASE,     (HALF_WIDTH_OF_ROAD  )+k, (LINES-BOTTOM_MARGIN-1)    , 14, VDP_CMD_IMP);
    // right half (page 1)
    vdp_cmd_execute_LINE(  0, HORIZON_BASE+256, (HALF_WIDTH_OF_ROAD  )+k, (LINES-BOTTOM_MARGIN-1)+256, 14, VDP_CMD_IMP);
    vdp_cmd_execute_LINE(  0, HORIZON_BASE+256, (HALF_WIDTH_OF_ROAD/3)+k, (LINES-BOTTOM_MARGIN-1)+256, 14, VDP_CMD_IMP);
  }

  // Draw the ground
  {
    // sea?
    vdp_cmd_execute_HMMV(0, 0,   256, HORIZON_BASE, 0x55);
    vdp_cmd_execute_HMMV(0, 256, 256, HORIZON_BASE, 0x55);
    // sky?
    vdp_cmd_execute_HMMV(0, 0,   256, HSYNC_LINE0, 0x77);
    vdp_cmd_execute_HMMV(0, 256, 256, HSYNC_LINE0, 0x77);
    uint8_t c = 0xcc;
    uint8_t h = 8;
    for (uint8_t i = (uint8_t)HORIZON_BASE; i < (uint8_t)LINES; ) {
      uint8_t hh = h >> 3;
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
  R.x = R.y = r.x = r.y = 0;
  x_pos = x_pos_d = vx = 0;

  idx = 0;
  const uint8_t r26 = hscroll_register_r26_value_from(CENTER); // R#26
  const uint8_t r27 = hscroll_register_r27_value_from(CENTER); // R#27
  uint8_t * p = (uint8_t *)scroll_table;
  for (uint8_t n = 2 * RASTER_ROWS; n--; ) {
    *p++ = 0;                   // R#23
    *p++ = r26;
    *p++ = r27;
  }

  memset(curvatures, 0, sizeof(curvatures));
  curvatures_idx = 0;
}

static palette_t color[16] = {
  [12] = RGB(2,2,5),
  [13] = RGB(1,1,4),
  [14] = RGB(0,0,0),
  [15] = RGB(7,7,7),
};

static void road_animation(void) {
  if (tick & 1) {
    vdp_set_palette(12, color[12]);
    vdp_set_palette(13, color[13]);
    vdp_set_palette(14, color[14]);
    vdp_set_palette(15, color[15]);
  }
  else {
    vdp_set_palette(12, color[13]);
    vdp_set_palette(13, color[12]);
    vdp_set_palette(14, color[15]);
    vdp_set_palette(15, color[14]);
  }
}

inline void generate_curve(void) {
  // generate lateral curve every 64 ticks
  if (!(tick & 63)) {
    uint16_t a = rand();
    if (!(a & 3)) {
      // return to straight course in 25% probability.
      R.x = 0;
      hide_curve_indicator();
    }
    else {
      a = (a >> 2) & 255;
      if (a < 2 * MAX_RX + 1) {
        // change target curvature `R.x` to random value in range of -MAX_RX to +MAX_RX
        R.x = a - MAX_RX;
        // show curve sign before reach to the curve if it is sharp curve.
        if (MAX_RX / 8 < R.x) {
          show_curve_indicator_right();
        }
        else if (R.x < -MAX_RX / 8) {
          show_curve_indicator_left();
        }
        else if (-MAX_RX / 16 < R.x && R.x < MAX_RX / 16) {
          hide_curve_indicator();
        }
      }
      else {
        // keep target curvature `R.x`
      }
    }
  }
}

inline void generate_undulation(void) {
  // generate up/down
  if (r.x <= -MAX_RX/8 || MAX_RX/8 <= r.x) {
    // reset targete curvature `R.y` to 0 in sharp curve
    R.y = 0;
  }
  else if (r.y == R.y) {
    uint16_t a = rand() & 255;
    if (a < (MAX_RY - MIN_RY) + 1) {
      // change target curvature `R.y` to random value in range of MIN_RY to MAX_RY
      R.y = a + MIN_RY;
    }
    else {
      // keep target curvature `R.y`
    }
  }
}

inline void generate_undulation_by_arrow_keys(void) {
  uint8_t joy = joypad_get_state(0);
  if (R.y < MAX_RY && (joy & VK_UP)) {
    R.y++;
  }
  if (MIN_RY < R.y && (joy & VK_DOWN)) {
    R.y--;
  }
}

static void update_road_geometry(void) {
  generate_curve();
  generate_undulation();
  // generate_undulation_by_arrow_keys();

  // Gradually bring the curvature closer to the target value.
  if (r.x < R.x) {
    r.x++;
  }
  if (R.x < r.x) {
    r.x--;
  }
  if (r.y < R.y) {
    // In the concave area (near the foot of the undulation), the change in
    // longitudinal curvature is gradual.
    r.y++;
    if (R.y < r.y) {
      r.y = R.y;
    }
  }
  if (R.y < r.y) {
    // In the convex area (near the apex of the undulations),
    // the change in longitudinal curvature is steep.
    r.y -= 8;
    if (r.y < R.y) {
      r.y = R.y;
    }
  }

  // update the ring buffer (history) of curvature
  if (!curvatures_idx) {
    curvatures[0].x = r.x;
    curvatures[0].y = r.y;
    curvatures[0].x_sum = curvatures[RASTER_ROWS-1].x_sum + r.x;
    curvatures[0].y_sum = curvatures[RASTER_ROWS-1].y_sum + r.y;
  }
  else {
    curvatures[curvatures_idx].x = r.x;
    curvatures[curvatures_idx].y = r.y;
    curvatures[curvatures_idx].x_sum = curvatures[curvatures_idx-1].x_sum + r.x;
    curvatures[curvatures_idx].y_sum = curvatures[curvatures_idx-1].y_sum + r.y;
  }
  curvatures_idx++;
  if (RASTER_ROWS <= curvatures_idx) {
    curvatures_idx = 0;
  }
  r0.x = curvatures[curvatures_idx].x;
  r0.y = curvatures[curvatures_idx].y;
  r0.x_sum = curvatures[curvatures_idx].x_sum;
  r0.y_sum = curvatures[curvatures_idx].y_sum;
}

inline void update_raster_scroll_table(void) {
  const uint8_t k = idx ^ 1;
  uint16_t x = x_pos * RASTER_ROWS;
  uint16_t y = r0.y;
  const struct curvature * p_curvatures = &curvatures[curvatures_idx];
  uint8_t j = curvatures_idx;
  uint8_t * p = ((uint8_t *)&scroll_table[k][RASTER_ROWS]) - 1;
  for (uint8_t i = RASTER_ROWS; i; i--) {
    x -= x_pos;
    // const uint16_t sx = CENTER + (x >> 7);
    // *p-- = hscroll_register_r27_value_from(sx); // R#27
    // *p-- = hscroll_register_r26_value_from(sx); // R#26
    // *p-- = 0;
    const int8_t sy = (int8_t)(y >> 8);
    const int16_t x_adjust = x_pos * sy / LINES_PER_ROW;
    const uint16_t sx = CENTER + ((x + x_adjust) >> 7) + 7;
    *p-- = ~((uint8_t)sx) & 7;          // R#27
    *p-- = ((uint8_t)(sx >> 3)) & 0x3f; // R#26
    *p-- = sy;
    x -= (int16_t)(p_curvatures->x_sum - r0.x_sum) * 8 / i;
    y += (int16_t)(p_curvatures->y_sum - r0.y_sum);
    p_curvatures++;
    j++;
    if (RASTER_ROWS <= j) {
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
  set_interrupt_handler(interrupt_handler);

  init_variables();
  show_vehicle();
  // main loop of the game
  tick = 0;
  for (;;) {
    tick++;
    await_vsync();
    road_animation();           // palette animation of the road.
    update_road_geometry();     // update curvature of the road.

    // update scroll amount for distant landscape
    bg_scroll_x_d -= r0.x;
    bg_scroll_x -= bg_scroll_x_d / 16;
    bg_scroll_x_d %= 16;

    bg_scroll_y = -((int8_t)r0.y) / 4;

    // steering
    uint8_t joy = joypad_get_state(0);
    if (joy & VK_RIGHT) {
      if (58 <= vx && vx < 64) {
        show_vehicle_R();
      }
      vx += 6;
    }
    else if (joy & VK_LEFT) {
      if (-64 < vx && vx <= -58) {
        show_vehicle_L();
      }
      vx -= 6;
    }
    else {
      if (0 < vx) {
        if (28 <= vx && vx < 32) {
          show_vehicle();
        }
        vx -= 4;
      }
      else if (vx < 0) {
        if (-32 < vx && vx <= -28) {
          show_vehicle();
        }
        vx += 4;
      }
    }
    if (96 < vx) {
      vx = 96;
    }
    else if (vx < -96) {
      vx = -96;
    }
    // slide vehicle's lateral position according to steering and road curvature.
    x_pos_d += vx;
    x_pos_d -= 2 * r0.x;
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
