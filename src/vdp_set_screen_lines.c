// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_screen_lines(enum vdp_screen_lines lines) {
  __critical {
    RG9SAV = (RG9SAV & ~0x80) | lines;
    VDP_SET_CTRL(9, RG9SAV);
  }
}
