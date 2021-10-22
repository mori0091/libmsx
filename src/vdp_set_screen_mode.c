// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_screen_mode(enum vdp_screen_mode mode) {
  uint8_t r0;                   /* 0| 0| 0| 0|M5|M4|M3| 0 */
  uint8_t r1;                   /* 0| 0| 0|M1|M2| 0| 0| 0 */
  switch (mode) {
  case VDP_SCREEN_MODE_TEXT_1:
    r0 = 0x00; r1 = 0x10;
    sprite_mode = 0;
    break;
  case VDP_SCREEN_MODE_TEXT_2:
    r0 = 0x08; r1 = 0x10;
    sprite_mode = 0;
    break;
  case VDP_SCREEN_MODE_MULTI_COLOR:
    r0 = 0x00; r1 = 0x08;
    sprite_mode = 1;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_1:
    r0 = 0x00; r1 = 0x00;
    sprite_mode = 1;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_2:
    r0 = 0x02; r1 = 0x00;
    sprite_mode = 1;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_3:
    r0 = 0x04; r1 = 0x00;
    sprite_mode = 2;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_4:
    r0 = 0x06; r1 = 0x00;
    sprite_mode = 2;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_5:
    r0 = 0x08; r1 = 0x00;
    sprite_mode = 2;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_6:
    r0 = 0x0A; r1 = 0x00;
    sprite_mode = 2;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_7:
    r0 = 0x0E; r1 = 0x00;
    sprite_mode = 2;
    break;
  default:
    return;
  }
  __critical {
    screen_mode = mode;
    RG0SAV = (RG0SAV & ~0x0e) | r0;
    RG1SAV = (RG1SAV & ~0x18) | r1;
    VDP_SET_CTRL(0, RG0SAV);
    VDP_SET_CTRL(1, RG1SAV);
  }
}
