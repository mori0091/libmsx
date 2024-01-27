// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_cmd_read.c
 */

#include "../include/vdp.h"

bool vdp_cmd_read(uint8_t * ret) {
  uint8_t s2;
  do {
    s2 = vdp_get_status(2);
    if (s2 & 0x80) {
      // TR == 1 (Transfer Ready)
      if (ret) {
        *ret = vdp_get_status(7);
      }
      return true;
    }
    // loop while CE == 1 (Command Executing)
  } while (s2 & 1);
  return false;
}
