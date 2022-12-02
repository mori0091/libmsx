// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file interrupt.h
 * \brief Interrupts relevant functions and Pluggable interrupts handler interface.
 * \ingroup INTERRUPT
 */
/**
 * \defgroup INTERRUPT Interrupts
 * `#include <interrupt.h>`
 */

#pragma once

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup INTERRUPT_AWAIT Await interrupts
 * \ingroup INTERRUPT
 * Await interrupts.
 * @{
 */

/**
 * `MSX` "Busy" flag for VSYNC interrupt routine.
 *
 * `vsync_busy` is set to `false` when the VSYNC interrupt routine finished.
 *
 * \note
 * This is used internally by `await_vsync()` to work with the VSYNC interrupt
 * routine. Normally, you don't need to worry about this. However, if you want
 * to override the default VSYNC interrupt routine without using
 * `set_vsync_handler()`, you have to clear it in the VSYNC interrupt routine.
 */
extern volatile bool vsync_busy;

/**
 * `MSX` Waits for next interrupt.
 *
 * Calling to `await_interrupt()` waits for next interrupt and blocks until the
 * interrupt would to be processed.
 *
 * \note
 * This waits for any type of interrupt including VSYNC.
 * If you want to wait for VSYNC, use await_vsync() instead.
 *
 * \note
 * This function is supposed to be used to synchronize with an interrupt in the
 * main loop of the application.
 *
 * \note
 * This function cannot be used from an interrupt handler; if you need an
 * interrupt handler for any type of interrupt, use set_interrupt_handler()
 * instead. Or if you need an interrupt handler for VSYNC, use
 * set_vsync_handler().
 *
 * \sa await_vsync()
 */
#define await_interrupt()                       \
  __asm__("ei");                                \
  __asm__("halt")

/**
 * `MSX` Waits for next VSYNC interrupt.
 *
 * Calling to `await_vsync()` waits for next VSYNC interrupt and blocks until
 * the interrupt would to be processed.
 *
 * \note
 * This function is supposed to be used to synchronize with VSYNC in the main
 * loop of the application.
 *
 * \note
 * This function cannot be used from an interrupt handler; if you need an
 * interrupt handler for VSYNC, use set_vsync_handler() instead.
 *
 * \sa await_interrupt()
 */
void await_vsync(void);

/** @} */

/**
 * \defgroup INTERRUPT_HANDLER Interrupts handler
 * \ingroup INTERRUPT
 * Pluggable interrupts handler interface.
 * @{
 */

/**
 * `MSX` Register user defined interrupt handler.
 *
 * After this function is called, the given user-defined handler will be called
 * from the H.KEYI hook each time an interrupt occurs. When the user-defined
 * handler returns, the original interrupt handler will be called. In other
 * words, this function inserts a function at the beginning of H.KEYI and chains
 * it to the original handler.
 *
 * If this function is called twice, the previous user-defined handler will be
 * replaced by the specified handler.
 *
 * If NULL is passed to this function, the registered user-defined handler will
 * be unregistered.
 *
 * \param handler  the user defined interrupt handler.
 */
void set_interrupt_handler(void (*handler)(void));

/**
 * `MSX` Register user defined VSYNC interrupt handler.
 *
 * After this function is called, the given user-defined handler will be called
 * from the H.TIMI hook each time a VSYNC interrupt occurs. When the
 * user-defined handler returns, the original VSYNC interrupt handler will be
 * called. In other words, this function inserts a function at the beginning of
 * H.TIMI and chains it to the original handler.
 *
 * If this function is called twice, the previous user-defined handler will be
 * replaced by the specified handler.
 *
 * If NULL is passed to this function, the registered user-defined handler will
 * be unregistered.
 *
 * \param handler  the user defined VSYNC interrupt handler.
 */
void set_vsync_handler(void (*handler)(void));

/** @} */

/**
 * \defgroup INTERRUPT_SLEEP Sleep
 * \ingroup INTERRUPT
 * Sleep functions.
 * @{
 */

/**
 * `MSX` Wait for the specified number of VSYNCs to occur.
 *
 * \param ticks   number of VSYNCs
 */
void sleep_ticks(uint16_t ticks);

/**
 * `MSX` Wait for the specified time to elapse in milliseconds.
 *
 * \param ms    time to elapse in milliseconds.
 */
void sleep_millis(uint16_t ms);

/** @} */

#endif
