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
 * \file chunk_list.h
 */

#ifndef CHUNK_LIST_H_
#define CHUNK_LIST_H_

#include "chunk.h"

#define List(T) List_ ## T
typedef struct List(Chunk) * List(Chunk);
struct List(Chunk) {
  List(Chunk) tail;
  Chunk head;
  int bits;
  uint16_t last_offset;
  size_t ref_cnt;
};

/**
 * Perform garbage collection.
 */
void run_gc(void);

/**
 * Delete the first node of the list.
 * \param xs a list.
 * \return tail of the list.
 */
List(Chunk) drop(List(Chunk) xs);

/**
 * Assign the list to a variable.
 * \param p  pointer to a variable.
 * \param xs a list.
 */
void assign(List(Chunk) * p, List(Chunk) xs);

/**
 * Construct a list.
 * \param x  head value.
 * \param xs tail.
 * \return A list starting with `x` followed by `xs`.
 */
List(Chunk) cons(Chunk x, List(Chunk) xs);

/**
 * Reverse the list.
 * \param xs a list.
 * \param The list rearranged in reverse order.
 * \post xs would point to the last node in the reversed list.
 */
List(Chunk) reverse(List(Chunk) xs);

#endif // CHUNK_LIST_H_
