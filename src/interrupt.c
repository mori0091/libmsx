// -*- coding: utf-8-unix -*-
/**
 * \file interrupt.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../include/interrupt.h"

static __at(0xfd9a) uint8_t H_KEYI[5];
static __at(0xfd9f) uint8_t H_TIMI[5];

static uint8_t hook_keyi[5];
static uint8_t hook_timi[5];

static void (*interrupt_handler)(void) = 0;
static void (*vsync_handler)(void) = 0;

static volatile bool vsync_not_finished;

static void default_interrupt_handler(void) {
  vsync_not_finished = true;
  if (interrupt_handler) {
    interrupt_handler();
  }
  __asm__("jp _hook_keyi");
}

static void default_vsync_handler(void) {
  if (vsync_handler) {
    vsync_handler();
  }
  __asm__("call _hook_timi");
  vsync_not_finished = false;
}

static void replacement_for_H_KEYI(void) {
  __asm__("jp _default_interrupt_handler");
  __asm__("nop");
  /* __asm__("nop"); */
}

static void replacement_for_H_TIMI(void) {
  __asm__("jp _default_vsync_handler");
  __asm__("nop");
  /* __asm__("nop"); */
}

void libmsx___init_intr(void) {
  __asm__("di");
  {
    // hook H.KEYI
    interrupt_handler = 0;
    memcpy(hook_keyi, H_KEYI, 5);
    memcpy(H_KEYI, replacement_for_H_KEYI, 5);
    // hook H.TIMI
    vsync_handler = 0;
    memcpy(hook_timi, H_TIMI, 5);
    memcpy(H_TIMI, replacement_for_H_TIMI, 5);
  }
  __asm__("ei");
}

void set_interrupt_handler(void (*handler)(void)) {
  __critical {
    interrupt_handler = handler;
  }
}

void set_vsync_handler(void (*handler)(void)) {
  __critical {
    vsync_handler = handler;
  }
}

void await_vsync(void) {
  do {
    await_interrupt();
  } while (vsync_not_finished);
}
