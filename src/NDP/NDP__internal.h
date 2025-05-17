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
 * \file NDP__internal.h
 */

#ifndef NDP__INTERNAL_H_
#define NDP__INTERNAL_H_

#include <memfile.h>

#include <stdbool.h>
#include <stdint.h>

#define bmem_offset_of(loc) (uint16_t)((loc) & (PAGE_SIZE - 1))

struct NDP_ {
  uint8_t song_bank;            // MegaROM segment of the current song.
  bool paused;
};

extern struct NDP_ NDP_;

extern void NDP_IMAIN(void);
extern void NDP_ADRSET(void * ptr);

extern void NDP__set_song_ptr(uint8_t bank, void * ptr);

extern bool NDP__load_data(MemFile * mf, void * buf, size_t buf_size);

#endif // NDP__INTERNAL_H_
