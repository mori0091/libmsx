// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_pattern_table(vmemptr_t table) {
  uint8_t r4;
  switch (screen_mode) {
  case VDP_SCREEN_MODE_TEXT_1:
  case VDP_SCREEN_MODE_TEXT_2:
  case VDP_SCREEN_MODE_MULTI_COLOR:
  case VDP_SCREEN_MODE_GRAPHIC_1:
    r4 = (table >> 11) & 0x3F;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_2:
  case VDP_SCREEN_MODE_GRAPHIC_3:
    r4 = ((table >> 11) & 0x3F) | 0x03;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_4:
  case VDP_SCREEN_MODE_GRAPHIC_5:
  case VDP_SCREEN_MODE_GRAPHIC_6:
  case VDP_SCREEN_MODE_GRAPHIC_7:
  default:
    return;
  }
  __critical {
    RG4SAV = r4;
    VDP_SET_CTRL(4, RG4SAV);
  }
}
