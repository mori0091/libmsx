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
 * \file NDP_start.c
 */

#include <NDP.h>
#include "./NDP__internal.h"

void NDP_start(void) {
  NDP_.paused = false;
  if (NDP_is_playing()) return;
  __asm__("push ix");
  __asm__("di");
  __asm__("call _NDP_MSTART");
  __asm__("ei");
  __asm__("pop ix");
}
