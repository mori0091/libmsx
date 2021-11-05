// -*- coding: utf-8-unix -*-

#ifndef PSG_H
#define PSG_H

#pragma once

#include <stdint.h>

void psg_init(void);

uint8_t psg_get(uint8_t reg);

void psg_set(uint8_t reg, uint8_t val);

#endif
