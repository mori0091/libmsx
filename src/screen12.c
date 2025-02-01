// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file screen12.c
 */

#include "screen.h"
#include <vdp.h>

void screen12(void) {
  screen8();
  vdp_set_yjk_mode(VDP_YJK);
}
