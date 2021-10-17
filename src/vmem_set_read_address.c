// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_set_read_address(vmemptr_t loc) {
  VDP_SET_CTRL(14, (uint8_t)(((loc) >> 14) & 7));
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F);
}
