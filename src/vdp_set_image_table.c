// -*- coding: utf-8-unix -*-
/**
 * \file vdp_set_image_table.c
 *
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

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
  __asm__("di");
  RG2SAV = r2;
  VDP_SET_CONTROL_REGISTER(2, RG2SAV);
  __asm__("ei");
}
