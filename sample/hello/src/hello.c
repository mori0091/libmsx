// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file hello.c
 */

#include <msx.h>
#include <screen.h>
#include <text.h>

static const char* MSX_PLATFORM_NAME[] = {
  [0] = "MSX",
  [1] = "MSX2",
  [2] = "MSX2+",
  [3] = "MSXtR",
};

void main(void) {
  screen1();
  color(15, 4, 7);

  uint8_t msx_ver = msx_get_version();
  if (3 < msx_ver) {
    msx_ver = 3;
  }

  locate(10, 7);
  print("HELLO ");
  print(MSX_PLATFORM_NAME[msx_ver]);

  // inifinite loop...
  for (;;) {
    await_vsync();
  }
}
