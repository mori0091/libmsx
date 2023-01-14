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
 * \file input.h
 * \brief Input devices such as Joypad, for example.
 * \ingroup INPUT
 */
/**
 * \defgroup INPUT Input device
 * `#include <input.h>`
 */

#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/**
 * \defgroup INPUT_JOYPAD Joystick / Joypad inteface
 * \ingroup INPUT
 * Get Joypad button state.
 * @{
 */

/**
 * Bitmask for UP direction of joystick or UP arrow key.
 *
 * \sa joypad_get_state()
 */
#define VK_UP     (1 << 0)

/**
 * Bitmask for DOWN direction of joystick or DOWN arrow key.
 *
 * \sa joypad_get_state()
 */
#define VK_DOWN   (1 << 1)

/**
 * Bitmask for LEFT direction of joystick or LEFT arrow key.
 *
 * \sa joypad_get_state()
 */
#define VK_LEFT   (1 << 2)

/**
 * Bitmask for RIGHT direction of joystick or RIGHT arrow key.
 *
 * \sa joypad_get_state()
 */
#define VK_RIGHT  (1 << 3)

/**
 * Bitmask for trigger button #1 or SPACE key
 *
 * \sa joypad_get_state()
 */
#define VK_FIRE_0 (1 << 4)

/**
 * Bitmask for trigger button #2
 *
 * \sa joypad_get_state()
 */
#define VK_FIRE_1 (1 << 5)

/**
 * `MSX` Get the status of the joystick buttons and levers.
 *
 * Get the status of the two trigger buttons and eight-way lever of the
 * specified joystick (or SPACE key and arrow keys of the keyboard).
 *
 * \param controller joystick number
 *           - `0` : keyboard (the arrow keys and the SPACE key)
 *           - `1` : joystick #1
 *           - `2` : joystick #2
 *
 * \return status of buttons.\n
 *         the following bits will be `1` if corresponding buttons / keys were
 *         pressed or will be `0` if released.
 *           - `bit #0` : VK_UP
 *           - `bit #1` : VK_DOWN
 *           - `bit #2` : VK_LEFT
 *           - `bit #3` : VK_RIGHT
 *           - `bit #4` : VK_FIRE_0
 *           - `bit #5` : VK_FIRE_1
 *
 * \note
 * Each bit of the return value is independent and dedicated to the
 * corresponding button/key. Therefore, a single call to joypad_get_state() can
 * be used to get the state of all buttons on the specified joystick.
 *
 * \note
 * For example, if you push the joystick lever to the upper right and press
 * trigger #1, bit #0, bit #3, and bit #4 of the return value will be set to 1
 * (i.e., the value `VK_UP | VK_RIGHT | VK_FIRE_0` will be returned).
 *
 * \sa msx_GTSTCK()
 * \sa msx_GTTRIG()
 */
uint8_t joypad_get_state(uint8_t controller);

/** @} */

#endif
