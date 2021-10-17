// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_write(vmemptr_t dst, void* src, uint16_t len) {
  __critical {
    vmem_set_write_address(dst);
    for (uint8_t* p = src; len--; ) {
      vdp_port0 = *p++;
    }
  }
}
