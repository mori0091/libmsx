// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp_set_yjk_mode.c
 */

#include "../include/vdp.h"

void vdp_set_yjk_mode(enum vdp_yjk_mode yjk) {
  __asm__("di");
  RG25SA &= ~0x18;
  RG25SA |= yjk;
  VDP_SET_CONTROL_REGISTER(25, RG25SA);
  __asm__("ei");
}
