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
 * \file sm2_update_sprite.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

#include <stdbool.h>
#include <stdint.h>

static bool update_frame(sm2_Sprite * s) {
  const uint8_t curr_frame = s->curr_frame;
  const sm2_FrameTag * const tag = s->tag;
  const uint8_t from = tag->from;
  const uint8_t to = tag->to;
  const uint8_t direction = tag->direction;

  if (curr_frame == to && direction == SM2_PINGPONG) {
    s->curr_direction = SM2_REVERSE;
  }
  else if (curr_frame == from && direction == SM2_PINGPONG_REVERSE) {
    s->curr_direction = SM2_FORWARD;
  }

  if (s->curr_direction == SM2_FORWARD) {
    if (curr_frame < to) {
      s->curr_frame++;
    }
    else if (direction == SM2_FORWARD || direction == SM2_PINGPONG_REVERSE) {
      return true;
    }
  }
  else {
    if (from < curr_frame) {
      s->curr_frame--;
    }
    else if (direction == SM2_REVERSE || direction == SM2_PINGPONG) {
      return true;
    }
  }
  return false;
}

void sm2_update_sprite(sm2_Sprite * s) {
  if (!s->remaining_duration) return; // duration = inf.
  s->remaining_duration -= SM2_COUNTS_PER_TICK;
  if (0 < s->remaining_duration) return;
  const sm2_FrameTag * const tag = s->tag;
  if (!tag) {
    s->remaining_duration = 0;
    return;
  }

  const sm2_SpriteSheet * const sheet = s->sheet;
  const bool cycle_end = update_frame(s);
  if (cycle_end) {
    const size_t repeat_max = tag->repeat;
    if (!repeat_max) {
      sm2_init_sprite(s, sheet, tag);
    }
    else {
      const size_t repeat = s->repeat;
      if (repeat < repeat_max) {
        sm2_init_sprite(s, sheet, tag);
        s->repeat = repeat + 1;
      }
      else {
        s->remaining_duration = 0;
      }
    }
  }
  else {
    size_t duration = SM2_COUNTS_PER_MSEC * sheet->frames.ptr[s->curr_frame].duration;
    s->remaining_duration += duration;
  }
}
