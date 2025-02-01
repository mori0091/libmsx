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
 * \file yjk.c
 * \brief `MSX2+` YJK colors
 */

#include <msx.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

void main(void) {
  screen12();
  color((uint8_t)31<<3, 0, 0);

  for (;;) {
    for (int8_t y = 0; y < 32; y++) {
      if (y % 3 == 0) {
        cls();
        locate(0, 25); print("(J,K), Y="); printi(y);
      }
      else {
        print(","); printi(y);
      }
      vmem_set_write_address((vmemptr_t)256 * 64 * (y % 3));
      for (int8_t k = -32; k < 32; k++) {
        for (int8_t j = -32; j < 32; j++) {
          vmem_set(((uint8_t)y << 3) | ((uint8_t)k & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)k >> 3) & 7));
          vmem_set(((uint8_t)y << 3) | ((uint8_t)j & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)j >> 3) & 7));
        }
      }
      if (y % 3 == 2) {
        sleep_millis(3000);
      }
    }
    sleep_millis(3000);

    for (int8_t k = -32; k < 32; k++) {
      if ((k+32) % 6 == 0) {
        cls();
        locate(0, 25); print("(J,Y), K="); printi(k);
      }
      else {
        print(","); printi(k);
      }
      vmem_set_write_address((vmemptr_t)256 * 32 * ((k+32) % 6));
      for (int8_t y = 0; y < 32; y++) {
        for (int8_t j = -32; j < 32; j++) {
          vmem_set(((uint8_t)y << 3) | ((uint8_t)k & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)k >> 3) & 7));
          vmem_set(((uint8_t)y << 3) | ((uint8_t)j & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)j >> 3) & 7));
        }
      }
      if ((k+32) % 6 == 5) {
        sleep_millis(3000);
      }
    }
    sleep_millis(3000);

    for (int8_t j = -32; j < 32; j++) {
      if ((j+32) % 6 == 0) {
        cls();
        locate(0, 25); print("(K,Y), J="); printi(j);
      }
      else {
        print(","); printi(j);
      }
      vmem_set_write_address((vmemptr_t)256 * 32 * ((j+32) % 6));
      for (int8_t y = 0; y < 32; y++) {
        for (int8_t k = -32; k < 32; k++) {
          vmem_set(((uint8_t)y << 3) | ((uint8_t)k & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)k >> 3) & 7));
          vmem_set(((uint8_t)y << 3) | ((uint8_t)j & 7));
          vmem_set(((uint8_t)y << 3) | (((uint8_t)j >> 3) & 7));
        }
      }
      if ((j+32) % 6 == 5) {
        sleep_millis(3000);
      }
    }
    sleep_millis(3000);
  }
}
