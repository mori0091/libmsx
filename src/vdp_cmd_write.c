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
 * \file vdp_cmd_write.c
 */

#include "../include/vdp.h"

uint8_t vdp_cmd_write_CMD = 0;

static bool vdp_cmd_write__first(const uint8_t val) {
  if (vdp_cmd_is_running()) {
    return false;
  }
  vdp_set_control(44, val);
  vdp_set_control(46, vdp_cmd_write_CMD);
  vdp_cmd_write_CMD = 0;
  return true;
}

static bool vdp_cmd_write__next(const uint8_t val) {
  uint8_t s2;
  do {
    s2 = vdp_get_status(2);
    if (!(s2 & 1)) {
      // CE == 0 (Command not Executing)
      return false;
    }
    // loop while TR == 0 (Transfer not Ready)
  } while (!(s2 & 0x80));
  vdp_set_control(44, val);
  return true;
}

bool vdp_cmd_write(const uint8_t val) {
  if (vdp_cmd_write_CMD) {
    return vdp_cmd_write__first(val);
  }
  else {
    return vdp_cmd_write__next(val);
  }
}
