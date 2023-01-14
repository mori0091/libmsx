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
 * \file scc_device.c
 */

#include "scc.h"
#include "scc_io.h"

#include <stdint.h>

const struct SCC_Device SCC_device = {
  .rw_channel_mask = &SCC_channel_mask,
  .wo_deformation  = &SCC_deformation,
  .channels = {
    [0] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCC_waveform[0][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCC_waveform[0][0],
      .rw_fdr      = &SCC_fdr[0],
      .rw_volume   = &SCC_volume[0],
    },
    [1] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCC_waveform[1][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCC_waveform[1][0],
      .rw_fdr      = &SCC_fdr[1],
      .rw_volume   = &SCC_volume[1],
    },
    [2] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCC_waveform[2][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCC_waveform[2][0],
      .rw_fdr      = &SCC_fdr[2],
      .rw_volume   = &SCC_volume[2],
    },
    [3] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCC_waveform[3][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCC_waveform[3][0], // shared with ch5
      .rw_fdr      = &SCC_fdr[3],
      .rw_volume   = &SCC_volume[3],
    },
    [4] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCC_waveform5[0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCC_waveform[3][0], // shared with ch4
      .rw_fdr      = &SCC_fdr[4],
      .rw_volume   = &SCC_volume[4],
    },
  },
};

const struct SCC_Device SCCPlus_device = {
  .rw_channel_mask = &SCCPlus_channel_mask,
  .wo_deformation  = &SCCPlus_deformation,
  .channels = {
    [0] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCCPlus_waveform[0][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCCPlus_waveform[0][0],
      .rw_fdr      = &SCCPlus_fdr[0],
      .rw_volume   = &SCCPlus_volume[0],
    },
    [1] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCCPlus_waveform[1][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCCPlus_waveform[1][0],
      .rw_fdr      = &SCCPlus_fdr[1],
      .rw_volume   = &SCCPlus_volume[1],
    },
    [2] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCCPlus_waveform[2][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCCPlus_waveform[2][0],
      .rw_fdr      = &SCCPlus_fdr[2],
      .rw_volume   = &SCCPlus_volume[2],
    },
    [3] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCCPlus_waveform[3][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCCPlus_waveform[3][0],
      .rw_fdr      = &SCCPlus_fdr[3],
      .rw_volume   = &SCCPlus_volume[3],
    },
    [4] = {
      .ro_waveform = (volatile const struct SCC_Waveform *)&SCCPlus_waveform[4][0],
      .wo_waveform = (volatile struct SCC_Waveform *)&SCCPlus_waveform[4][0],
      .rw_fdr      = &SCCPlus_fdr[4],
      .rw_volume   = &SCCPlus_volume[4],
    },
  },
};
