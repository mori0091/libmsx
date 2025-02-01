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
 * \file vdp_unsafe.h
 * \brief Unsafe primitive functions for VDP (Video Display Proccessor) access.
 * \ingroup VDP
 */

#pragma once

#ifndef VDP_UNSAFE_H_
#define VDP_UNSAFE_H_

#include <stdint.h>

#include "io.h"
#include "vmem.h"
#include "bios.h"

/**
 * \defgroup VDP_UNSAFE Unsafe primitive functions
 * \ingroup VDP
 * Unsafe primitive functions for VDP (Video Display Proccessor) access.
 * @{
 */

/**
 * `MSX` Write a value to the specified VDP control register.
 *
 * \param reg  VDP control register number.
 * \param val  Value to be written.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa vdp_set_control()
 */
inline void VDP_SET_CONTROL_REGISTER(uint8_t reg, uint8_t val) {
  vdp_port1 = val;
  vdp_port1 = reg | 0x80;
}

/**
 * `MSX2` Set VDP control register number for subsequent writing to the VDP
 * control register.
 *
 * \param reg  VDP control register number.
 *
 * \attention
 * Unsafe but fast.
 *
 * \note
 * Same as `VDP_SET_CONTROL_REGISTER(17, reg | 0x80)`.
 *
 * \sa VDP_SET_CONTROL_REGISTER_VALUE()
 */
inline void VDP_SET_CONTROL_REGISTER_POINTER(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(17, reg | 0x80);
}

/**
 * `MSX2` Set VDP control register number for subsequent writing to the series
 * of VDP control registers.
 *
 * \param reg  VDP control register number.
 *
 * \attention
 * Unsafe but fast.
 *
 * \note
 * Same as `VDP_SET_CONTROL_REGISTER(17, reg)`.
 *
 * \sa VDP_SET_CONTROL_REGISTER_VALUE()
 * \sa vdp_write_control()
 */
inline void VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(17, reg);
}

/**
 * `MSX2` Write a value to the VDP control register pointed by R#17.
 *
 * \param val  a value.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa VDP_SET_CONTROL_REGISTER_POINTER()
 * \sa VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT()
 */
inline void VDP_SET_CONTROL_REGISTER_VALUE(uint8_t val) {
  vdp_port3 = val;
}

/**
 * `MSX2` Set VDP status register number for subsequent reading from the VDP
 * status register.
 *
 * \param reg  VDP status register number.
 *
 * \attention
 * Unsafe but fast.
 *
 * \note
 * Same as `VDP_SET_CONTROL_REGISTER(15, reg)`.
 *
 * \sa VDP_GET_STATUS_REGISTER_VALUE()
 * \sa vdp_get_status()
 */
inline void VDP_SET_STATUS_REGISTER_POINTER(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(15, reg);
}

/**
 * `MSX` Reads a value from VDP status register.
 *
 * \return a value.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa vdp_get_status()
 */
inline uint8_t VDP_GET_STATUS_REGISTER_VALUE(void) {
  return vdp_port1;
}

/**
 * `MSX` Set VRAM address for subsequent writing to VRAM.
 *
 * \param loc  VRAM address.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa VDP_SET_VMEM_VALUE()
 * \sa vmem_set_write_address()
 */
inline void VDP_SET_VMEM_WRITE_POINTER(vmemptr_t loc) {
  if (0 < msx_get_version()) {
    VDP_SET_CONTROL_REGISTER(14, (uint8_t)(((loc) >> 14) & 7));
  }
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F | 0x40);
}

/**
 * `MSX` Set VRAM address for subsequent reading from VRAM.
 *
 * \param loc  VRAM address.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa VDP_GET_VMEM_VALUE()
 * \sa vmem_set_read_address()
 */
inline void VDP_SET_VMEM_READ_POINTER(vmemptr_t loc) {
  if (0 < msx_get_version()) {
    VDP_SET_CONTROL_REGISTER(14, (uint8_t)(((loc) >> 14) & 7));
  }
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F);
}

/**
 * `MSX` Write a value to VRAM.
 *
 * \param val  a value.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa VDP_SET_VMEM_WRITE_POINTER()
 * \sa vmem_set()
 * \sa vmem_write()
 * \sa vmem_memset()
 */
inline void VDP_SET_VMEM_VALUE(uint8_t val) {
  vdp_port0 = val;
}

/**
 * `MSX` Read a value from VRAM.
 *
 * \return a value.
 *
 * \attention
 * Unsafe but fast.
 *
 * \sa VDP_SET_VMEM_READ_POINTER()
 * \sa vmem_get()
 * \sa vmem_read()
 */
inline uint8_t VDP_GET_VMEM_VALUE(void) {
  return vdp_port0;
}

/** @} */

#endif // VDP_UNSAFE_H_
