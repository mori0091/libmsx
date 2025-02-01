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
 * \file scc_wav.h
 * \brief Predefined waveforms for SCC/SCC+.
 * \ingroup SCC
 */

#pragma once

#ifndef SCC_WAV_H_
#define SCC_WAV_H_

#include <stdint.h>

/**
 * \defgroup SCC_PREDEFINED_WAVEFORMS Predefined waveforms for SCC/SCC+.
 * \ingroup SCC
 * `#include <scc_wav.h>`
 * Predefined waveforms for SCC/SCC+.
 * @{
 */

extern const int8_t SCC_WAVEFORM_PULSE_31_32[32]; ///< Pulse wave. (duty ratio = 31/32)
extern const int8_t SCC_WAVEFORM_PULSE_30_32[32]; ///< Pulse wave. (duty ratio = 30/32)
extern const int8_t SCC_WAVEFORM_PULSE_29_32[32]; ///< Pulse wave. (duty ratio = 29/32)
extern const int8_t SCC_WAVEFORM_PULSE_28_32[32]; ///< Pulse wave. (duty ratio = 28/32)
extern const int8_t SCC_WAVEFORM_PULSE_27_32[32]; ///< Pulse wave. (duty ratio = 27/32)
extern const int8_t SCC_WAVEFORM_PULSE_26_32[32]; ///< Pulse wave. (duty ratio = 26/32)
extern const int8_t SCC_WAVEFORM_PULSE_25_32[32]; ///< Pulse wave. (duty ratio = 25/32)
extern const int8_t SCC_WAVEFORM_PULSE_24_32[32]; ///< Pulse wave. (duty ratio = 24/32)
extern const int8_t SCC_WAVEFORM_PULSE_23_32[32]; ///< Pulse wave. (duty ratio = 23/32)
extern const int8_t SCC_WAVEFORM_PULSE_22_32[32]; ///< Pulse wave. (duty ratio = 22/32)
extern const int8_t SCC_WAVEFORM_PULSE_21_32[32]; ///< Pulse wave. (duty ratio = 21/32)
extern const int8_t SCC_WAVEFORM_PULSE_20_32[32]; ///< Pulse wave. (duty ratio = 20/32)
extern const int8_t SCC_WAVEFORM_PULSE_19_32[32]; ///< Pulse wave. (duty ratio = 19/32)
extern const int8_t SCC_WAVEFORM_PULSE_18_32[32]; ///< Pulse wave. (duty ratio = 18/32)
extern const int8_t SCC_WAVEFORM_PULSE_17_32[32]; ///< Pulse wave. (duty ratio = 17/32)
extern const int8_t SCC_WAVEFORM_PULSE_16_32[32]; ///< Pulse wave. (duty ratio = 16/32)
extern const int8_t SCC_WAVEFORM_PULSE_15_32[32]; ///< Pulse wave. (duty ratio = 15/32)
extern const int8_t SCC_WAVEFORM_PULSE_14_32[32]; ///< Pulse wave. (duty ratio = 14/32)
extern const int8_t SCC_WAVEFORM_PULSE_13_32[32]; ///< Pulse wave. (duty ratio = 13/32)
extern const int8_t SCC_WAVEFORM_PULSE_12_32[32]; ///< Pulse wave. (duty ratio = 12/32)
extern const int8_t SCC_WAVEFORM_PULSE_11_32[32]; ///< Pulse wave. (duty ratio = 11/32)
extern const int8_t SCC_WAVEFORM_PULSE_10_32[32]; ///< Pulse wave. (duty ratio = 10/32)
extern const int8_t SCC_WAVEFORM_PULSE_9_32[32]; ///< Pulse wave. (duty ratio = 9/32)
extern const int8_t SCC_WAVEFORM_PULSE_8_32[32]; ///< Pulse wave. (duty ratio = 8/32)
extern const int8_t SCC_WAVEFORM_PULSE_7_32[32]; ///< Pulse wave. (duty ratio = 7/32)
extern const int8_t SCC_WAVEFORM_PULSE_6_32[32]; ///< Pulse wave. (duty ratio = 6/32)
extern const int8_t SCC_WAVEFORM_PULSE_5_32[32]; ///< Pulse wave. (duty ratio = 5/32)
extern const int8_t SCC_WAVEFORM_PULSE_4_32[32]; ///< Pulse wave. (duty ratio = 4/32)
extern const int8_t SCC_WAVEFORM_PULSE_3_32[32]; ///< Pulse wave. (duty ratio = 3/32)
extern const int8_t SCC_WAVEFORM_PULSE_2_32[32]; ///< Pulse wave. (duty ratio = 2/32)
extern const int8_t SCC_WAVEFORM_PULSE_1_32[32]; ///< Pulse wave. (duty ratio = 1/32)

#define SCC_WAVEFORM_PULSE_15_16 SCC_WAVEFORM_PULSE_30_32
#define SCC_WAVEFORM_PULSE_14_16 SCC_WAVEFORM_PULSE_28_32
#define SCC_WAVEFORM_PULSE_13_16 SCC_WAVEFORM_PULSE_26_32
#define SCC_WAVEFORM_PULSE_12_16 SCC_WAVEFORM_PULSE_24_32
#define SCC_WAVEFORM_PULSE_11_16 SCC_WAVEFORM_PULSE_22_32
#define SCC_WAVEFORM_PULSE_10_16 SCC_WAVEFORM_PULSE_20_32
#define SCC_WAVEFORM_PULSE_9_16  SCC_WAVEFORM_PULSE_18_32
#define SCC_WAVEFORM_PULSE_8_16  SCC_WAVEFORM_PULSE_16_32
#define SCC_WAVEFORM_PULSE_7_16  SCC_WAVEFORM_PULSE_14_32
#define SCC_WAVEFORM_PULSE_6_16  SCC_WAVEFORM_PULSE_12_32
#define SCC_WAVEFORM_PULSE_5_16  SCC_WAVEFORM_PULSE_10_32
#define SCC_WAVEFORM_PULSE_4_16  SCC_WAVEFORM_PULSE_8_32
#define SCC_WAVEFORM_PULSE_3_16  SCC_WAVEFORM_PULSE_6_32
#define SCC_WAVEFORM_PULSE_2_16  SCC_WAVEFORM_PULSE_4_32
#define SCC_WAVEFORM_PULSE_1_16  SCC_WAVEFORM_PULSE_2_32

#define SCC_WAVEFORM_PULSE_7_8  SCC_WAVEFORM_PULSE_28_32
#define SCC_WAVEFORM_PULSE_6_8  SCC_WAVEFORM_PULSE_24_32
#define SCC_WAVEFORM_PULSE_5_8  SCC_WAVEFORM_PULSE_20_32
#define SCC_WAVEFORM_PULSE_4_8  SCC_WAVEFORM_PULSE_16_32
#define SCC_WAVEFORM_PULSE_3_8  SCC_WAVEFORM_PULSE_12_32
#define SCC_WAVEFORM_PULSE_2_8  SCC_WAVEFORM_PULSE_8_32
#define SCC_WAVEFORM_PULSE_1_8  SCC_WAVEFORM_PULSE_4_32

#define SCC_WAVEFORM_PULSE_3_4  SCC_WAVEFORM_PULSE_24_32
#define SCC_WAVEFORM_PULSE_2_4  SCC_WAVEFORM_PULSE_16_32
#define SCC_WAVEFORM_PULSE_1_4  SCC_WAVEFORM_PULSE_8_32

#define SCC_WAVEFORM_PULSE_1_2  SCC_WAVEFORM_PULSE_16_32

#define SCC_WAVEFORM_SQUARE     SCC_WAVEFORM_PULSE_16_32

extern const int8_t SCC_WAVEFORM_TRIANGLE[32]; ///< Trianguler wave

extern const int8_t SCC_WAVEFORM_SIN[32]; ///< Sine wave

/** @} */

#endif // SCC_WAV_H_
