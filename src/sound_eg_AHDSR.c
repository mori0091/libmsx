// -*- coding: utf-8-unix -*-
/**
 * \file sound_eg_AHDSR.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "sound_eg_spi.h"

#define END_OF_PHASE  (0)
#define ATTACK_PHASE  (1)
#define HOLD_PHASE    (2)
#define DECAY_PHASE   (3)
#define SUSTAIN_PHASE (4)
#define RELEASE_PHASE (5)

#define ATTACK_RATE   (self->attr.ahdsr->attack_rate)
#define HOLD_TIME     (self->attr.ahdsr->hold_time)
#define DECAY_RATE    (self->attr.ahdsr->decay_rate)
#define SUSTAIN_LEVEL (self->attr.ahdsr->sustain_level)
#define RELEASE_RATE  (self->attr.ahdsr->release_rate)

static void attack_phase(struct sound_eg * self) {
  if (ATTACK_RATE < 255 - self->output) {
    self->output += ATTACK_RATE;
    return;
  }
  self->output = 255;
  if (HOLD_TIME) {
    self->time = HOLD_TIME;
    self->state = HOLD_PHASE;
  }
  else {
    self->state = DECAY_PHASE;
  }
}

static void hold_phase(struct sound_eg * self) {
  if (!self->time--) {
    self->state = DECAY_PHASE;
  }
}

static void decay_phase(struct sound_eg * self) {
  if (DECAY_RATE < self->output) {
    self->output -= DECAY_RATE;
  }
  else {
    self->output = 0;
  }
  if (self->output <= SUSTAIN_LEVEL) {
    self->output = SUSTAIN_LEVEL;
    self->state = SUSTAIN_PHASE;
  }
}

static void sustain_phase(struct sound_eg * self) {
  // nothing to do
  ((void)self);
}

static void release_phase(struct sound_eg * self) {
  if (RELEASE_RATE < self->output) {
    self->output -= RELEASE_RATE;
    return;
  }
  self->output = 0;
  self->state = END_OF_PHASE;
}

// ----

static void sound_eg_AHDSR_key_on(struct sound_eg * self) {
  self->state = ATTACK_PHASE;
  self->output = 0;
  attack_phase(self);
}

static void sound_eg_AHDSR_key_off(struct sound_eg * self) {
  self->state = RELEASE_PHASE;
  release_phase(self);
}

static void sound_eg_AHDSR_advance(struct sound_eg * self) {
  switch (self->state) {
  case END_OF_PHASE:
    return;
  case ATTACK_PHASE:
    attack_phase(self);
    return;
  case HOLD_PHASE:
    hold_phase(self);
    return;
  case DECAY_PHASE:
    decay_phase(self);
    return;
  case SUSTAIN_PHASE:
    sustain_phase(self);
    return;
  case RELEASE_PHASE:
    release_phase(self);
    return;
  default:
    return;
  }
}

const struct sound_eg_vtbl sound_eg_AHDSR = {
  .key_on  = sound_eg_AHDSR_key_on,
  .key_off = sound_eg_AHDSR_key_off,
  .advance = sound_eg_AHDSR_advance,
};
