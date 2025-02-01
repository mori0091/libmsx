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
 * \file snd_a.h
 *
 */

#ifndef SND_A_H_
#define SND_A_H_

#include <stdint.h>
#include <stdbool.h>

#include "../include/snd_p_table.h"

#define snd_a_note_on(ctx) snd_p_note_on(ctx)
#define snd_a_note_off(ctx) snd_p_note_off(ctx)
#define snd_a__program_change(ctx, p) snd_p__program_change(ctx, p)
#define snd_a__decode(ctx) snd_p__decode(ctx)

#endif // SND_A_H_
