// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_image_table(vmemptr_t table) {
  uint8_t r2;
  switch (screen_mode) {
  case VDP_SCREEN_MODE_TEXT_2:
    r2 = ((table >> 10) & 0x7F) | 0x03;
    break;
  case VDP_SCREEN_MODE_TEXT_1:
  case VDP_SCREEN_MODE_MULTI_COLOR:
  case VDP_SCREEN_MODE_GRAPHIC_1:
  case VDP_SCREEN_MODE_GRAPHIC_2:
  case VDP_SCREEN_MODE_GRAPHIC_3:
    r2 = (table >> 10) & 0x7F;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_4:
  case VDP_SCREEN_MODE_GRAPHIC_5:
    r2 = ((table >> 10) & 0x7F) | 0x1F;
    break;
  case VDP_SCREEN_MODE_GRAPHIC_6:
  case VDP_SCREEN_MODE_GRAPHIC_7:
    r2 = ((table >> 11) & 0x3F) | 0x1F;
    break;
  default:
    return;
  }
  __critical {
    RG2SAV = r2;
    VDP_SET_CTRL(2, RG2SAV);
  }
}
