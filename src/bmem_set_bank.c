// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem_get_bank.c
 */

#include "bmem.h"

void bmem_set_bank(uint8_t bank) __naked {
  (void)bank;
  __asm__("jp set_bank");
}
