// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_sprite_attribute_table(vmemptr_t table) {
  uint8_t r5;
  uint8_t r11;
  switch (screen_mode) {
  case VDP_SCREEN_MODE_TEXT_1:
  case VDP_SCREEN_MODE_TEXT_2:
    return;
  case VDP_SCREEN_MODE_MULTI_COLOR:
  case VDP_SCREEN_MODE_GRAPHIC_1:
  case VDP_SCREEN_MODE_GRAPHIC_2:
    r5 = (table >>  7) & 0xFF;
    r11 = (table >> 15) & 0x03;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_3:
  case VDP_SCREEN_MODE_GRAPHIC_4:
  case VDP_SCREEN_MODE_GRAPHIC_5:
  case VDP_SCREEN_MODE_GRAPHIC_6:
  case VDP_SCREEN_MODE_GRAPHIC_7:
    r5 = ((table >> 7) & 0xFF) | 0x07;
    r11 = (table >> 15) & 0x03;
    break;
  default:
    return;
  }
  __critical {
    RG5SAV = r5;
    RG11SA = r11;
    VDP_SET_CTRL(5, RG5SAV);
    VDP_SET_CTRL(11, RG11SA);
  }
}
