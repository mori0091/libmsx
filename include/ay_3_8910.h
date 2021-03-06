// -*- coding: utf-8-unix -*-
/**
 * \file ay_3_8910.h
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef AY_3_8910_H
#define AY_3_8910_H

#include "psg.h"

extern uint8_t ay_3_8910_buffer[14];

void ay_3_8910_init(void);

void ay_3_8910_play(void);

#endif
