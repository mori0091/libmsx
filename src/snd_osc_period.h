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
 * \file snd_osc_period.h
 *
 */

#ifndef SND_OSC_PERIOD_H_
#define SND_OSC_PERIOD_H_

#include <stdint.h>

#define OSC_PERIODS_INDEX_MAX        (3 + 8 * 12) // A0, A#0, B0, C1..B8
#define MIDI_NOTE_NUMBER_MIN         (21) // A0
#define MIDI_NOTE_NUMBER_MAX         (MIDI_NOTE_NUMBER_MIN + OSC_PERIODS_INDEX_MAX - 1)
#define MIDI_NOTE_NUMBER_TO_INDEX(x) ((x) - MIDI_NOTE_NUMBER_MIN)

#define PITCH_MIN                    (MIDI_NOTE_NUMBER_MIN << 8)
#define PITCH_MAX                    (MIDI_NOTE_NUMBER_MAX << 8)

int16_t snd_osc_period(int16_t pitch);

#endif // SND_OSC_PERIOD_H_
