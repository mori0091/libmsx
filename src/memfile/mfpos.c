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
 * \file mfpos.c
 */

#include <memfile.h>
#include <bios.h>

#define bmem_offset_of(loc) (uint16_t)((loc) & (PAGE_SIZE - 1))

uint32_t mfpos(const struct MemPos * pos) {
  return pos->slot
    ? (uint32_t)pos->segment * PAGE_SIZE + bmem_offset_of((uintptr_t)pos->ptr)
    : (uintptr_t)pos->ptr;
}
