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
 * \file slot_iterate.c
 */

#include "slot.h"

#include "workarea.h"

/**
 * For each slot, invoke the given callback with the given arguments.
 *
 * \param callback  a user defined callback function that takes `slot` and `arg`.
 * \param arg       argument to be passed to the `callback`.
 */
void slot_iterate(void (*callback)(uint8_t slot, void * arg), void * arg) {
  for (uint8_t i = 0; i < 4; ++i) {
    uint8_t slot = i;
    if (EXPTBL[i] & 0x80) {
      slot |= 0x80;
      for (uint8_t j = 0; j < 4; ++j) {
        callback(slot | (j << 2), arg);
      }
    }
    else {
      callback(slot, arg);
    }
  }
}
