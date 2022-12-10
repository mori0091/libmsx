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
 * \file slot_is_SCCPlus.c
 */

#include "slot.h"

#include "scc.h"

bool slot_is_SCCPlus(uint8_t slot) {
  return (1 < SCC_inspect(slot));
}