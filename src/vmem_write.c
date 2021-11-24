// -*- coding: utf-8-unix -*-
/**
 * \file vmem_write.c
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

// void vmem_write(vmemptr_t dst, void* src, uint16_t len) {
//   __critical {
//     vmem_set_write_address(dst);
//     for (uint8_t* p = src; len--; ) {
//       vdp_port0 = *p++;
//     }
//   }
// }

void vmem_write(vmemptr_t dst, void* src, uint16_t len) {
  __critical {
    VDP_SET_VMEM_WRITE_POINTER(dst);
  }
  for (uint8_t* p = src; len--; ) {
    VDP_SET_VMEM_VALUE(*p++);
  }
}
