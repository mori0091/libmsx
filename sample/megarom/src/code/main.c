// -*- coding: utf-8-unix -*-
/**
 * \file main.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

// #pragma codeseg CODE

#include "boot_main.h"
#include "game_main.h"

void main(void) {
  boot_main();                  // call boot_main() __banked function in BANK1
  game_main();                  // call game_main() __banked function in BANK0
}
