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
 * \file NDP_pause.c
 */

#include <NDP.h>
#include "./NDP__internal.h"
#include "psg.h"

void NDP_pause(void) {
  NDP_.paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}
