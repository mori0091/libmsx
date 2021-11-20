// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_sprite_attribute_table.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

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
    VDP_SET_CONTROL_REGISTER(5, RG5SAV);
    if (0 < msx_get_version()) {
      RG11SA = r11;
      VDP_SET_CONTROL_REGISTER(11, RG11SA);
    }
  }
}
