// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_color_table.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_color_table(vmemptr_t table) {
  uint8_t r3;
  uint8_t r10;
  switch (screen_mode) {
    case VDP_SCREEN_MODE_TEXT_1:
      return;
    case VDP_SCREEN_MODE_TEXT_2:
      r3 = ((table >> 6) & 0xFF) | 0x07;
      r10 = (table >> 14) & 0x07;
      break;
    case VDP_SCREEN_MODE_MULTI_COLOR:
      return;
    case VDP_SCREEN_MODE_GRAPHIC_1:
      r3 = (table >> 6) & 0xFF;
      r10 = (table >> 14) & 0x07;
      break;
    case VDP_SCREEN_MODE_GRAPHIC_2:
    case VDP_SCREEN_MODE_GRAPHIC_3:
      r3 = ((table >> 6) & 0xFF) | 0x7F;
      r10 = (table >> 14) & 0x07;
      break;
    case VDP_SCREEN_MODE_GRAPHIC_4:
    case VDP_SCREEN_MODE_GRAPHIC_5:
    case VDP_SCREEN_MODE_GRAPHIC_6:
    case VDP_SCREEN_MODE_GRAPHIC_7:
    default:
      return;
  }
  __asm__("di");
  RG3SAV = r3;
  VDP_SET_CONTROL_REGISTER(3, RG3SAV);
  if (0 < msx_get_version()) {
    RG10SA = r10;
    VDP_SET_CONTROL_REGISTER(10, RG10SA);
  }
  __asm__("ei");
}
