// -*- coding: utf-8-unix -*-
/**
 * \file vdp_cmd_execute_HMMV.c
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

void vdp_cmd_execute_HMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color) {
  vdp_cmd_await();
  __critical {
    VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(36);
    VDP_SET_CONTROL_REGISTER_VALUE(x & 0xff);         /* R#36 */
    VDP_SET_CONTROL_REGISTER_VALUE((x >> 8) & 0x01);  /* R#37 */
    VDP_SET_CONTROL_REGISTER_VALUE(y & 0xff);         /* R#38 */
    VDP_SET_CONTROL_REGISTER_VALUE((y >> 8) & 0x03);  /* R#39 */
    VDP_SET_CONTROL_REGISTER_VALUE(w & 0xff);         /* R#40 */
    VDP_SET_CONTROL_REGISTER_VALUE((w >> 8) & 0x01);  /* R#41 */
    VDP_SET_CONTROL_REGISTER_VALUE(h & 0xff);         /* R#42 */
    VDP_SET_CONTROL_REGISTER_VALUE((h >> 8) & 0x03);  /* R#43 */
    VDP_SET_CONTROL_REGISTER_VALUE(color);            /* R#44 */
    VDP_SET_CONTROL_REGISTER_VALUE(0);                /* R#45 */
    VDP_SET_CONTROL_REGISTER_VALUE(VDP_CMD_HMMV);     /* R#46 */
  }
}
