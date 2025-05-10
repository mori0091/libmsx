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
 * \file NDP_stop.c
 */

#include <NDP.h>

void NDP_stop(void) {
  __asm__("push ix");
  __asm__("di");
  __asm__("call _NDP_MSTOP");
  __asm__("ei");
  __asm__("pop ix");
}
