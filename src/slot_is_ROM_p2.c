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
 * \file slot_is_ROM_p2.c
 */

#include "slot.h"

bool slot_is_ROM_p2(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x8000, "AB", 2));
}
