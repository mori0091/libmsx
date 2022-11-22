// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file slot_is_BDOS.c
 */

#include "slot.h"

#include "workarea.h"

bool slot_is_BDOS(uint8_t slot) {
  if (!DRVTBL[0]) return false;
  const volatile uint8_t * p = DRVTBL;
  while (*p) {
    if (slot == p[1]) {
      return true;
    }
    p += 2;
  }
  return false;
}
