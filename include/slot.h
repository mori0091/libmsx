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
 * \file slot.h
 * \brief Utility functions to inspect the slot mechanism of MSX.
 *
 * *Example*\n
 * The following code detects MSX-MUSIC (OPLL) by inspecting each slot.
 * ~~~ c
 * #include <msx.h>
 * #include <slot.h>
 *
 * void find_OPLL_callback(uint8_t slot, void * arg) {
 *   uint8_t * p_slot = (uint8_t *)arg;
 *   if (slot_is_internal_OPLL(slot)) {
 *     *p_slot = slot;
 *   }
 *   else if (!*p_slot && slot_is_OPLL(slot)) {
 *     *p_slot = slot;
 *   }
 * }
 *
 * void main(void) {
 *   uint8_t slot = 0;
 *   slot_iterate(find_OPLL_callback, &slot);
 *   if (slot) {
 *     // a MSX-MUSIC (OPLL) found on the slot `slot`.
 *   }
 *   for (;;) {
 *     await_vsync();
 *   }
 * }
 * ~~~
 */

#pragma once

#ifndef SLOT_H_
#define SLOT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * `MSX` For each slot, invoke the given callback with the given arguments.
 *
 * \param callback  a user defined callback function that takes `slot` and `arg`.
 * \param arg       argument to be passed to the `callback`.
 */
void slot_iterate(void (*callback)(uint8_t slot, void * arg), void * arg);

/**
 * `MSX` Compare the byte sequence to the one present at the given address in
 * the given slot.
 *
 * \param slot  slot address of the one compared to.
 * \param addr  address of the one compared to.
 * \param s     pointer to the byte sequence.
 * \param len   length of the byte sequence.
 *
 * \return 0 if two byte sequence is same (or len was 0), otherwise non-zero value.
 */
int slot_bcmp(uint8_t slot, const void * addr, const void * s, size_t len);

/**
 * `MSX` Tests whether a given memory address of the given slot is read-only.
 *
 * This tests, in exact, whether a value read from the address is differ from
 * the value written before.
 *
 * In other words, if it returns `false`, it means that the memory address is
 * RAM or a typical R/W register. However, if it returns `true`, it does not
 * necessarily mean that the memory address is read-only, but could be a
 * write-only register, for example.
 *
 * \param slot  slot address
 * \param addr  address
 *
 * \return `true` if it seems not a RAM, `false` otherwise.
 */
bool slot_is_read_only(uint8_t slot, void * addr);

/**
 * `MSX` Inspects if the slot is MAIN ROM.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is MAIN ROM.
 */
bool slot_is_MAIN_ROM(uint8_t slot);

/**
 * `MSX` Inspects if the slot is SUB ROM.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is SUB ROM.
 */
bool slot_is_SUB_ROM(uint8_t slot);

/**
 * `MSX` Inspects if the slot is RAM.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is RAM.
 *
 * \note
 * Since page 3 of the default internal RAM is always selected, this test only
 * tests if the specified slot is the same as the slot selected on page 3
 * (0xC000.0xFFFF). However, maybe there is extra RAM in other slots.
 */
bool slot_is_RAM(uint8_t slot);

/**
 * `MSX` Inspects if the slot is the internal MSX-MUSIC.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is the internal MSX-MUSIC.
 */
bool slot_is_internal_OPLL(uint8_t slot);

/**
 * `MSX` Inspects if the slot is the FMPAC.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is the FMPAC.
 */
bool slot_is_FMPAC(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a MSX-MUSIC.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a MSX-MUSIC.
 */
bool slot_is_OPLL(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a Konami SCC/SCC+.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a Konami SCC/SCC+.
 */
bool slot_is_SCC(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a Konami SCC+.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a Konami SCC+.
 */
bool slot_is_SCCPlus(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a BDOS/FDC.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a BDOS/FDC.
 */
bool slot_is_BDOS(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a ROM starting at 0x4000.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a ROM starting at 0x4000.
 *
 * \note
 * This test only tests if a ROM header exists at 0x4000 of the specified slot.
 */
bool slot_is_ROM_p1(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a ROM starting at 0x8000.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a ROM starting at 0x8000.
 *
 * \note
 * This test only tests if a ROM header exists at 0x8000 of the specified slot.
 */
bool slot_is_ROM_p2(uint8_t slot);

/**
 * `MSX` Inspects if the slot is a ROM starting at 0x4000 or 0x8000.
 *
 * \param slot  a slot address.
 * \return `true` if the slot is a ROM starting at 0x4000 or 0x8000.
 *
 * \note
 * This test only tests if a ROM header exists at 0x4000 or 0x8000 of the
 * specified slot.
 */
bool slot_is_ROM(uint8_t slot);

#endif // SLOT_H_
