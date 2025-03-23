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
 * \file scc.h
 * \brief Device interface for Konami SCC/SCC+ sound cartridge.
 * \ingroup SCC
 *
 * *Example*\n
 * The following code detects the Konami SCC/SCC+ sound chip and plays a
 * triangular wave test tone if detected.
 * \include scc_example/src/scc_example.c
 */
/**
 * \defgroup SCC Konami SCC/SCC+ sound cartridge
 * \ingroup LIBMSX_SOUNDCHIP
 * `#include <scc.h>`
 */

#pragma once

#ifndef SCC_H_
#define SCC_H_

#include <stdint.h>

/**
 * \defgroup SCC_DEVICE SCC/SCC+ device interface
 * \ingroup SCC
 * `#include <scc.h>`
 * Inspect, discover, enable/disable to access registers, of the SCC/SCC+ sound chip.
 *
 * *Example*\n
 * The following code detects the Konami SCC/SCC+ sound chip and plays a
 * triangular wave test tone if detected.
 * \include scc_example/src/scc_example.c
 *
 * @{
 */

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
 * \sa SCC_set_mode()
 * \sa SCC_get_mode()
 * \sa SCC_enable()
 * \sa SCC_disable()
 */
struct SCC {
  uint8_t slot;                 ///< Slot address of the SCC/SCC+.
  uint8_t version;              ///< `1` if SCC, `2` or greater value if SCC+.
  uint8_t mode;                 ///< `1` if SCC compatible mode, `2` if SCC+ mode.
  const struct SCC_Device * device; ///< Pointer to device interface.
};

/**
 * `MSX` Inspect whether SCC/SCC+ is on the given slot.
 *
 * \param slot  a slot address
 *
 * \return `0` if neither SCC or SCC+, `1` if SCC, `2` or greater value if SCC+.
 */
uint8_t SCC_inspect(uint8_t slot);

/**
 * `MSX` Find SCC/SCC+ sound chip.
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
 *        - the handle is set to SCC compatible mode even if SCC+.
 *
 * \sa SCC_set_mode()
 * \sa SCC_get_mode()
 * \sa SCC_enable()
 * \sa SCC_disable()
 */
uint8_t SCC_find(struct SCC * scc);

/**
 * `MSX` Set the SCC handle to SCC compatible mode or SCC+ mode.
 *
 * The `scc` shall point to a SCC handle initialized by SCC_find().
 *
 * If `scc->version` is less than `2` (i.e., not SCC+), do nothing.
 *
 * The mode set by this function shall be applied to the sound chip the next
 * time SCC_enable() is called.
 *
 * \param scc  pointer to the SCC handle.
 * \param mode `1` for SCC mode, `2` for SCC+ mode, others are ignored.
 */
void SCC_set_mode(struct SCC * scc, uint8_t mode);

/**
 * `MSX` Check which mode the SCC handle is set to.
 *
 * The `scc` shall point to a SCC handle initialized by SCC_find().
 *
 * The mode returned by this function (i.e., the mode set to the SCC handle) may
 * not the current mode of the sound chip. The mode set to the SCC handle shall
 * be applied to the sound chip when SCC_enable() is called.
 *
 * \param scc  pointer to the SCC handle.
 * \return `1` if SCC mode, `2` if SCC+ mode, `0` otherwise.
 */
uint8_t SCC_get_mode(struct SCC * scc);

/**
 * `MSX` Enable SCC/SCC+ sound chip.
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
 * `MSX` Disable SCC/SCC+ sound chip.
 *
 * Unexpose SCC/SCC+ sound chip on the slot `scc->slot` and disable to access
 * registers of the sound chip.
 *
 * The `scc` shall point to a SCC handle initialized by SCC_find().
 *
 * \param scc  pointer to the SCC handle.
 */
void SCC_disable(const struct SCC * scc);

/** @} */

#endif // SCC_H_
