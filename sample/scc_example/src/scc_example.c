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
 * \file scc_example.c
 *
 * \brief
 * Detects the Konami SCC/SCC+ sound chip and plays a triangular wave test tone
 * if detected.
 */

#include <string.h>

#include <msx.h>
#include <scc.h>

const int8_t triangle[32] = {
  0, 16, 32, 48, 64, 80, 96, 112,
  127, 112, 96, 80, 64, 48, 32, 16,
  0, -16, -32, -48, -64, -80, -96, -112,
  -128, -112, -96, -80, -64, -48, -32, -16,
};

static struct SCC scc;

void test_SCC(void) {
  if (!scc.slot) return;
  SCC_enable(&scc);
  {
    uint8_t slot_p2 = msx_get_slot((void *)0x8000);
    msx_ENASLT(scc.slot, (void *)0x8000);
    {
      memcpy((void *)scc.device->channels[0].wo_waveform, triangle, 32);
      *scc.device->rw_channel_mask = 0x01; // unmute ch1
      *scc.device->channels[0].rw_volume = 15;
      *scc.device->channels[0].rw_fdr = 0x11d; // O4 G
    }
    msx_ENASLT(slot_p2, (void *)0x8000);
    __asm__("ei");
  }
  SCC_disable(&scc);
}

void main(void) {
  if (SCC_find(&scc)) {
    test_SCC();
  }
  for (;;) {
    await_vsync();
  }
}
