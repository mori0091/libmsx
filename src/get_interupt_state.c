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
 * \file get_interrupt_state.c
 */

#include <stdbool.h>
#include <config.h>

bool get_interrupt_state(void) __naked {
  __asm__("call    GETIFF");
  __asm__("ld      a, #1");
  __asm__("ret     nc");
  __asm__("xor     a");
  __asm__("ret");
}
