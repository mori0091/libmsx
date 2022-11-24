// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file scc.h
 * \brief Device interface for Konami SCC/SCC+ sound cartridge.
 *
 * *Example*\n
 * The following code detects the Konami SCC/SCC+ sound chip and plays a
 * triangular wave test tone if detected.
 * \include scc_example/src/scc_example.c
 */

#pragma once

#ifndef SCC_H_
#define SCC_H_

#include <stdint.h>

/**
 * The structure of the SCC/SCC+ 32-byte waveform data register.
 */
struct SCC_Waveform {
  int8_t data[32];
};

/**
 * Device interface for a sound channel of SCC/SCC+ sound chip.
 */
struct SCC_Channel {
  /**
   * Pointer to read data from the 32-byte waveform data register.
   * \note Read Only.
   */
  volatile const struct SCC_Waveform * ro_waveform;
  /**
   * Pointer to write data to the 32-byte waveform data register.
   * \note Write Only.
   */
  volatile struct SCC_Waveform * wo_waveform;
  /**
   * Pointer to read/write data from/to the frequency division ratio register.
   */
  volatile uint16_t * rw_fdr;
  /**
   * Pointer to read/write data from/to the volume register.
   */
  volatile uint8_t * rw_volume;
};

/**
 * Device interface for SCC/SCC+ sound chip.
 */
struct SCC_Device {
  /**
   * Pointer to read/write a value from/to the channel mask register.
   */
  volatile uint8_t * rw_channel_mask;
  /**
   * Pointer to write a value to the deformation register.
   * \note Write Only.
   */
  volatile uint8_t * wo_deformation;
  /**
   * Device interface for each 5 channels.
   */
  struct SCC_Channel channels[5];
};

/**
 * SCC Handle.
 *
 * \sa SCC_find()
 * \sa SCC_enable()
 * \sa SCC_disable()
 */
struct SCC {
  uint8_t slot;                 ///< Slot address of the SCC/SCC+.
  uint8_t version;              ///< `1` if SCC, `2` or greater value if `SCC+`.
  const struct SCC_Device * device; ///< Pointer to device interface.
};

/**
 * Inspect whether SCC/SCC+ is on the given slot.
 *
 * \param slot  a slot address
 *
 * \return `0` if neither SCC or SCC+, `1` if SCC, `2` or greater value if SCC+.
 */
uint8_t SCC_inspect(uint8_t slot);

/**
 * Find SCC/SCC+ sound chip.
 *
 * \param scc [out]  pointer to a SCC handle to be initialized.
 *
 * \return the slot address of SCC/SCC+ if found, `0` otherwise.
 *
 * \post  If no SCC/SCC+ found, `0` is set to `scc->slot`.
 * \post  If a SCC/SCC+ found;
 *        - the slot address of SCC/SCC+ is set to `scc->slot`,
 *        - `1` is set to `scc->version` if SCC, or
 *        - `2` or greater value is set to `scc->version` if SCC+.
 *        - the device interface is set to `scc->device`.
 */
uint8_t SCC_find(struct SCC * scc);

/**
 * Enable SCC/SCC+ sound chip.
 *
 * Expose SCC/SCC+ sound chip on the slot `scc->slot` and enable to access
 * registers of the sound chip by inter-slot read/write BIOS function.
 *
 * The `scc` shall point to a SCC handle initialized by SCC_find().
 *
 * \param scc  pointer to the SCC handle.
 */
void SCC_enable(const struct SCC * scc);

/**
 * Disable SCC/SCC+ sound chip.
 *
 * Unexpose SCC/SCC+ sound chip on the slot `scc->slot` and disable to access
 * registers of the sound chip.
 *
 * The `scc` shall point to a SCC handle initialized by SCC_find().
 *
 * \param scc  pointer to the SCC handle.
 */
void SCC_disable(const struct SCC * scc);

#endif // SCC_H_
