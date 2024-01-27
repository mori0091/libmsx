// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file sprites.h
 * \brief SPRITE part of `MSX2+` driving game demo.
 */

#ifndef SPRITES_H_
#define SPRITES_H_

void setup_sprites(void);

void hide_curve_indicator(void);
void show_curve_indicator_right(void);
void show_curve_indicator_left(void);

void hide_vehicle(void);
void show_vehicle(void);
void show_vehicle_L(void);
void show_vehicle_R(void);

#endif // SPRITES_H_
