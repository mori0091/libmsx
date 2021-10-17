// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2) {
  vdp_cmd_await();
  __critical {
    VDP_SET_CTRL_PTR(32);
    VDP_SET_CTRL_VALUE(x & 0xff);         /* R#32 */
    VDP_SET_CTRL_VALUE((x >> 8) & 0x01);  /* R#33 */
    VDP_SET_CTRL_VALUE(y & 0xff);         /* R#34 */
    VDP_SET_CTRL_VALUE((y >> 8) & 0x03);  /* R#35 */
    VDP_SET_CTRL_VALUE(x2 & 0xff);        /* R#36 */
    VDP_SET_CTRL_VALUE((x2 >> 8) & 0x01); /* R#37 */
    VDP_SET_CTRL_VALUE(y2 & 0xff);        /* R#38 */
    VDP_SET_CTRL_VALUE((y2 >> 8) & 0x03); /* R#39 */
    VDP_SET_CTRL_VALUE(w & 0xff);         /* R#40 */
    VDP_SET_CTRL_VALUE((w >> 8) & 0x01);  /* R#41 */
    VDP_SET_CTRL_VALUE(h & 0xff);         /* R#42 */
    VDP_SET_CTRL_VALUE((h >> 8) & 0x03);  /* R#43 */
    VDP_SET_CTRL_VALUE(0);                /* R#44 (unused) */
    VDP_SET_CTRL_VALUE(0);                /* R#45 */
    VDP_SET_CTRL_VALUE(VDP_CMD_HMMM);     /* R#46 */
  }
}
