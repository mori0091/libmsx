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
 * \file slot_is_ROM.c
 */

#include "slot.h"

bool slot_is_ROM(uint8_t slot) {
  return (slot_is_ROM_p1(slot) || slot_is_ROM_p2(slot));
}
