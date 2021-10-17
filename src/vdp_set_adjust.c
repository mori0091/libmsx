// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_adjust(int8_t x, int8_t y) {
  if (x < -7) x = -7;
  if (y < -7) y = -7;
  if (8 < x) x = 8;
  if (8 < y) y = 8;
  x = (0 < x) ? 16 - x : -x;
  y = (0 < y) ? 16 - y : -y;
  __critical {
    RG18SA = (y << 4) | x;
    VDP_SET_CTRL(18, RG18SA);
  }
}
