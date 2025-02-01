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
 * \file optimize.c
 */

#include "zx0.h"

#include "array.h"
#include "info.h"

#include <stdlib.h>

// -----------------------------------------------------------------------
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

/**
 * Returns the upper limit of offset values that can be used at a given
 * position.
 * \param index Next position of output in bytes.
 * \return The upper limit of offset values.
 */
uint16_t max_offset_at(int index) {
  return MAX(INITIAL_OFFSET, MIN(index, MAX_OFFSET));
}

/**
 * Returns the number of bits of Elias Gamma.
 * \param x An integer value.
 * \return The number of bits in the corresponding Elias Gamma code.
 */
int bit_length_of_elias_gamma(int x) {
  int ret = 1;
  while (x >>= 1) {
    ret += 2;
  }
  return ret;
}

// -----------------------------------------------------------------------
/**
 * Construct a list of Chunk that represents optimal compression.
 * \param beg Pointer to the head of the input (inclusive).
 * \param end Pointer to the end of the input (out of range).
 * \return A list that represents optimal compression.
 */
List(Chunk) optimize(const uint8_t * const beg, const uint8_t * const end) {
  if (beg >= end) {
    return NULL;
  }
  const int input_length = end - beg;
  const uint16_t max_offset_0 = max_offset_at(input_length - 1);
  List(Chunk) * optimal = array(1 + input_length, sizeof(List(Chunk))); // optimal List(Chunk) for each index
  List(Chunk) * literal = array(1 + max_offset_0, sizeof(List(Chunk))); // List(Chunk) for each offset, which ends with literal
  List(Chunk) * matched = array(1 + max_offset_0, sizeof(List(Chunk))); // List(Chunk) for each offset, which ends with matched
  int * match_length = array(1 + max_offset_0, sizeof(int)); // length of match for each offset
  {
    const Chunk dummy = {.bits_cost = -1, .index = 0, .offset = INITIAL_OFFSET};
    const List(Chunk) cs = cons(dummy, NULL);
    cs->last_offset = last_offset;
    assign(&matched[INITIAL_OFFSET], cs);
  }
  for (const uint8_t * p = beg; p != end; ++p) {
    const int index = p - beg;
    const uint16_t max_offset = max_offset_at(index);
    for (uint16_t offset = 1; offset <= max_offset; ++offset) {
      if (offset <= index && p[0] == p[-offset]) {
        if (literal[offset]) {
          // COPY FROM LAST OFFSET
          const List(Chunk) tail = literal[offset];
          const uint16_t len = index+1 - tail->head.index;
          // const Chunk c = MatchFromLastOffset(index+1, offset, len);
          const Chunk c
            = (tail->last_offset == offset)
            ? MatchFromLastOffset(index+1, offset, len)
            : MatchFromNewOffset(index+1, offset, len);
          const List(Chunk) cs = cons(c, tail);
          cs->last_offset = offset;
          assign(&matched[offset], cs);
          if (!optimal[index+1] || cs->bits < optimal[index+1]->bits) {
            assign(&optimal[index+1], cs);
          }
        }
        if (++match_length[offset] > 1) {
          // COPY FROM NEW OFFSET
          uint16_t len = match_length[offset];
          {
            int bits = optimal[index+1 - len]->bits + bit_length_of_elias_gamma(len - 1);
            for (int n = len - 1; 1 < n; --n) {
              int bits2 = optimal[index+1 - n]->bits + bit_length_of_elias_gamma(n - 1);
              if (bits2 < bits) {
                bits = bits2;
                len = n;
              }
            }
          }
          const List(Chunk) tail = optimal[index+1 - len];
          const Chunk c = MatchFromNewOffset(index+1, offset, len);
          if (!matched[offset]
              || matched[offset]->head.index != index+1
              || c.bits_cost + tail->bits < matched[offset]->bits) {
            const List(Chunk) cs = cons(c, tail);
            cs->last_offset = offset;
            assign(&matched[offset], cs);
            if (!optimal[index+1] || cs->bits < optimal[index+1]->bits) {
              assign(&optimal[index+1], cs);
            }
          }
        }
      }
      else {
        // COPY LITERAL
        match_length[offset] = 0;
        if (matched[offset]) {
          const List(Chunk) tail = matched[offset];
          const uint16_t len = index+1 - tail->head.index;
          const Chunk c = Literal(index+1, len);
          const List(Chunk) cs = cons(c, tail);
          cs->last_offset = tail->last_offset;
          assign(&literal[offset], cs);
          if (!optimal[index+1] || cs->bits < optimal[index+1]->bits) {
            assign(&optimal[index+1], cs);
          }
        }
      }
    }
    INFO("\r\033[0K%5.1f%% (%d/%d)", 100.0 * (index+1) / input_length, index+1, input_length);
  }
  INFO("\n");

  List(Chunk) cs = optimal[input_length];
  cs->ref_cnt--;
  optimal[input_length] = NULL;
  {
    // clean up
    for (int i = 0; i < input_length; ++i) {
      assign(&optimal[i], NULL);
    }
    free(optimal);

    // for (int i = 0; i <= max_offset_0; ++i) {
    //   assign(&literal[i], NULL);
    //   assign(&matched[i], NULL);
    // }
    free(literal);
    free(matched);
    free(match_length);
  }

  // assert(reverse(cs)->head == dummy);
  return drop(reverse(cs));
}
