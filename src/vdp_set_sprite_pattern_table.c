// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_sprite_pattern_table(vmemptr_t table) {
  uint8_t r6;
  switch (screen_mode) {
  case VDP_SCREEN_MODE_TEXT_1:
  case VDP_SCREEN_MODE_TEXT_2:
    return;
  case VDP_SCREEN_MODE_MULTI_COLOR:
  case VDP_SCREEN_MODE_GRAPHIC_1:
  case VDP_SCREEN_MODE_GRAPHIC_2:
  case VDP_SCREEN_MODE_GRAPHIC_3:
  case VDP_SCREEN_MODE_GRAPHIC_4:
  case VDP_SCREEN_MODE_GRAPHIC_5:
  case VDP_SCREEN_MODE_GRAPHIC_6:
  case VDP_SCREEN_MODE_GRAPHIC_7:
    r6 = (table >> 11) & 0x3F;
    break;
  default:
    return;
  }
  __critical {
    RG6SAV = r6;
    VDP_SET_CTRL(6, RG6SAV);
  }
}
