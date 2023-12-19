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
 * \file audio_efx_psg2scc.c
 */

#include <audio_efx_psg2scc.h>

#include <ay_3_8910.h>
#include <scc_buf.h>

void audio_efx_psg2scc(void) {
  scc_buffer.channel_mask = 0x1f;
  scc_buffer.fdr[0] = (uint16_t)ay_3_8910_buffer[0] + ay_3_8910_buffer[1] * 256;
  scc_buffer.fdr[1] = (uint16_t)ay_3_8910_buffer[2] + ay_3_8910_buffer[3] * 256;
  scc_buffer.fdr[2] = (uint16_t)ay_3_8910_buffer[4] + ay_3_8910_buffer[5] * 256;
  scc_buffer.volume[0] = (ay_3_8910_buffer[8]  < 16 ? ay_3_8910_buffer[8]  : 15);
  scc_buffer.volume[1] = (ay_3_8910_buffer[9]  < 16 ? ay_3_8910_buffer[9]  : 15);
  scc_buffer.volume[2] = (ay_3_8910_buffer[10] < 16 ? ay_3_8910_buffer[10] : 15);
}
