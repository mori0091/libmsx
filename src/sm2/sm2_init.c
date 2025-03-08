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
 * \file sm2_init.c
 */

#include <sm2.h>

#include <workarea.h>

void sm2_init(void) {
  sm2_cfg.vmem.spt = ((vmemptr_t)(RG6SAV & 0x3f) << 11);
  sm2_cfg.vmem.sat = ((vmemptr_t)(RG11SA & 0x03) << 15) | ((vmemptr_t)(RG5SAV & 0xfc) << 7);
}
