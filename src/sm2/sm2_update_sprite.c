// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2026 Daishi Mori (mori0091)
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
  const sm2_FrameTag * const tag = s->tag;

  if (s->curr_direction == SM2_FORWARD) {
    if (s->curr_frame < tag->to) {
      s->curr_frame++;
    }
    else if (tag->direction == SM2_PINGPONG) {
      s->curr_direction = SM2_REVERSE;
    }
    else {
      return true;
    }
  }
  else {
    if (tag->from < s->curr_frame) {
      s->curr_frame--;
    }
    else if (tag->direction == SM2_PINGPONG_REVERSE) {
      s->curr_direction = SM2_FORWARD;
    }
    else {
      return true;
    }
  }
  return false;
}

extern void sm2_init_sprite_0(sm2_Sprite * s, const sm2_SpriteSheet * sheet, const sm2_FrameTag * tag);

void sm2_update_sprite(sm2_Sprite * s) {
  if (!s->remaining_duration) return; // duration = inf.
  if (s->remaining_duration > SM2_COUNTS_PER_TICK) {
    s->remaining_duration -= SM2_COUNTS_PER_TICK;
    return;
  }
  s->remaining_duration = 0;
  const sm2_FrameTag * const tag = s->tag;
  if (!tag) {
    return;
  }

  const sm2_SpriteSheet * const sheet = s->sheet;
  if (update_frame(s)) {
    const size_t repeat_max = tag->repeat;
    if (!repeat_max) {
      sm2_init_sprite_0(s, sheet, tag);
    }
    else {
      const size_t repeat = s->repeat;
      if (repeat < repeat_max) {
        sm2_init_sprite_0(s, sheet, tag);
        s->repeat = repeat + 1;
      }
    }
  }
  else {
    size_t duration = SM2_COUNTS_PER_MSEC * sheet->frames.ptr[s->curr_frame].duration;
    s->remaining_duration = duration;
  }
}
