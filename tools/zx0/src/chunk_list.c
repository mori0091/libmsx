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
 * \file chunk_list.c
 */

#include "chunk_list.h"

#include "info.h"

#include <stdlib.h>

static List(Chunk) free_list = 0;

/**
 * Perform garbage collection.
 */
void run_gc(void) {
  while (free_list) {
    List(Chunk) tail = free_list->tail;
    free(free_list);
    free_list = tail;
  }
}

/**
 * Delete the first node of the list.
 * \param xs a list.
 * \return tail of the list.
 */
List(Chunk) drop(List(Chunk) xs) {
  List(Chunk) tail = xs->tail;
  if (tail && tail->ref_cnt) {
    tail->ref_cnt--;
  }
  if (!xs->ref_cnt) {
    xs->tail = free_list;
    free_list = xs;
  }
  return tail;
}

/**
 * Assign the list to a variable.
 * \param p  pointer to a variable.
 * \param xs a list.
 */
void assign(List(Chunk) * p, List(Chunk) xs) {
  if (*p) {
    List(Chunk) cs = *p;
    if (cs->ref_cnt) {
      cs->ref_cnt--;
    }
    while (cs && !cs->ref_cnt) {
      cs = drop(cs);
    }
  }
  if (xs) {
    xs->ref_cnt++;
  }
  *p = xs;
}

static List(Chunk) allocate(void) {
  List(Chunk) cs;
  if (free_list) {
    cs = free_list;
    free_list = cs->tail;
  }
  else {
    cs = malloc(sizeof(struct List(Chunk)));
    if (!cs) {
      ERROR("not enough memory");
    }
  }
  return cs;
}

/**
 * Construct a list.
 * \param x  head value.
 * \param xs tail.
 * \return A list starting with `x` followed by `xs`.
 */
List(Chunk) cons(Chunk x, List(Chunk) xs) {
  List(Chunk) node = allocate();
  node->head = x;
  node->bits = x.bits_cost;
  node->ref_cnt = 0;
  node->tail = 0;
  if (xs) {
    node->bits += xs->bits;
    assign(&node->tail, xs);
  }
  return node;
}

/**
 * Reverse the list.
 * \param xs a list.
 * \param The list rearranged in reverse order.
 * \post xs would point to the last node in the reversed list.
 */
List(Chunk) reverse(List(Chunk) xs) {
  List(Chunk) rev = 0;
  xs->ref_cnt++;
  while (xs) {
    List(Chunk) tail = xs->tail;
    xs->tail = rev;
    rev = xs;
    xs = tail;
  }
  rev->ref_cnt--;
  return rev;
}
