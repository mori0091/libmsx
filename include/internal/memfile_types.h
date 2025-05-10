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
 * \file memfile_types.h
 */

#ifndef MEMFILE_TYPES_H_
#define MEMFILE_TYPES_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Internal structure of mempos_t type.
 */
struct MemPos {
  uint8_t * ptr;                ///< address on the CPU address space.
  uint8_t segment;              ///< ROM / RAM mapper segment number.
  uint8_t slot;                 ///< slot address.
};

/**
 * Internal structure of MemFile type.
 */
struct MemFile {
  struct MemPos base;                    ///< base position
  struct MemPos curr;                    ///< current position
  struct MemPos end;                     ///< last position (exclusive)
  const struct MemFileMethods * methods; ///< function pointers.
};

struct MemFileMethods {
  uint8_t (* read1)(struct MemFile * mf);
  size_t (* read)(struct MemFile * mf, void * ptr, size_t size);
  void (* seek)(struct MemPos * dst, const struct MemPos * src, long offset);
};

uint32_t mfpos(const struct MemPos * pos);
uint32_t mfsize(struct MemFile * mf);

#endif // MEMFILE_TYPES_H_
