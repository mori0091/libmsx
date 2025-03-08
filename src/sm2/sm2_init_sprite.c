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
 * \file sm2_init_sprite.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

void sm2_init_sprite(sm2_Sprite * s, const sm2_SpriteSheet * sheet, const sm2_FrameTag * tag) {
  s->sheet = sheet;
  s->tag = tag;
  if (tag) {
    switch (tag->direction) {
      case SM2_FORWARD:
      case SM2_PINGPONG:
        s->curr_frame = tag->from;
        s->curr_direction = SM2_FORWARD;
        break;
      case SM2_REVERSE:
      case SM2_PINGPONG_REVERSE:
        s->curr_frame = tag->to;
        s->curr_direction = SM2_REVERSE;
        break;
    }
  }
  else {
    s->curr_frame = 0;
    s->curr_direction = SM2_FORWARD;
  }
  size_t duration = SM2_COUNTS_PER_MSEC * sheet->frames.ptr[s->curr_frame].duration;
  s->remaining_duration = duration;
  s->repeats = 0;
}
