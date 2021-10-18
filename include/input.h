// -*- coding: utf-8-unix -*-

#ifndef INPUT_H
#define INPUT_H

#pragma once

#include <assert.h>
#include <stdint.h>

#define VK_UP     (1 << 0)
#define VK_DOWN   (1 << 1)
#define VK_LEFT   (1 << 2)
#define VK_RIGHT  (1 << 3)
#define VK_FIRE_0 (1 << 4)
#define VK_FIRE_1 (1 << 5)

uint8_t joypad_get_state(uint8_t controller);

#endif
