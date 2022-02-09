// -*- coding: utf-8-unix -*-
/**
 * \file ay_3_8910.h
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef AY_3_8910_H
#define AY_3_8910_H

#pragma once

#include "psg.h"

extern uint8_t ay_3_8910_buffer[14];

void ay_3_8910_init(void);

void ay_3_8910_play(void);

#endif
