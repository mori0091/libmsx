// -*- coding: utf-8-unix -*-

#pragma once

#ifndef ROM_MAPPER_H_
#define ROM_MAPPER_H_

#include <stdint.h>

/**
 * Switch 32KiB ROM banks and then restart.
 *
 * \param bank    bank number
 */
void rom_banked_jump(uint8_t bank);

#endif // ROM_MAPPER_H_
