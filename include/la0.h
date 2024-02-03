// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file la0.h
 * \brief The libmsx audio format 0 (LA0) decoder.
 */

#ifndef LA0_H_
#define LA0_H_

#include <audio.h>
#include <audio_dec.h>
#include <memfile.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * LA0 BGM decoder.
 */
extern const AudioDecoder LA0_BGM_DECODER;

/**
 * LA0 SFX decoder.
 */
extern const AudioDecoder LA0_SFX_DECODER;

/**
 * `MSX` Open LA0 file stored in ROM / RAM.
 *
 * \param mf   Pointer to a memory-file pointer to be initialized.
 * \param loc  Location of the LA0 file image.
 * \param size File size in bytes.
 * \return     Number of songs contained in the LA0 file.
 */
int la0_open_mem(MemFile * mf, const uint8_t * loc, size_t size);

/**
 * `MSX` Open LA0 file stored in banked memory (MegaROM).
 *
 * \param mf   Pointer to a memory-file pointer to be initialized.
 * \param loc  Location of the LA0 file image in banked memory.
 * \param size File size in bytes.
 * \return     Number of songs contained in the LA0 file.
 */
int la0_open_bmem(MemFile * mf, bmemptr_t loc, uint32_t size);

/**
 * `MSX` Open LA0 file stored as named resources in banked memory (MegaROM).
 *
 * \param mf   Pointer to a memory-file pointer to be initialized.
 * \param path Path/File name of the resource.
 * \return     Number of songs contained in the LA0 file.
 */
int la0_open_resource(MemFile * mf, const char * path);

/**
 * `MSX` Set a song in the LA0 file to the LA0 audio decoder, as background
 * music (BGM).
 *
 * \param idx  Song number.
 * \param mf   Pointer to opened memory-file pointer.
 */
void la0_set_bgm(uint8_t idx, MemFile * mf);

/**
 * `MSX` Set a song in the LA0 file to the LA0 audio decoder, as sound effects
 * (SFX).
 *
 * \param idx  Song number.
 * \param mf   Pointer to opened memory-file pointer.
 */
void la0_set_sfx(uint8_t idx, MemFile * mf);

/**
 * `MSX` Set a song in the LA0 file to the LA0 audio decoder, as sound effects
 * (SFX).
 *
 * If a SFX is not being played, or if the given priority is higher than or
 * equal to that of the currently playing SFX, the specified SFX will be played.
 * Otherwise, nothing will be done.
 *
 * \param idx  Song number.
 * \param mf       Pointer to opened memory-file pointer.
 * \param priority Priority of the SFX.
 * \sa la0_open()
 */
void la0_set_sfx_with_priority(uint8_t idx, MemFile * mf, uint8_t priority);

/**
 * `MSX` Return default frequency of the current background music.
 *
 * \return frequency in Hz.
 * \sa audio_get_bgm_frequency()
 * \sa audio_set_bgm_frequency()
 */
uint8_t la0_get_bgm_frequency(void);

/**
 * `MSX` Return the total number of BGM samples.
 *
 * Returns the total number of samples of the current BGM (including the loop
 * part).
 *
 * The total time (in seconds) of the first part plus the loop part at normal
 * playback speed is `la0_get_bgm_total_samples() / la0_get_bgm_frequency()`.
 *
 * \return The total number of BGM samples.
 */
uint16_t la0_get_bgm_total_samples(void);

/**
 * `MSX` Return the number of samples in the loop portion of BGM.
 *
 * Returns the number of samples in the loop part of the current BGM, or `0` if
 * there is no loop part.
 *
 * The time (in seconds) of the loop part at normal playback speed is
 * `la0_get_bgm_loop_samples() / la0_get_bgm_frequency()`.
 *
 * \return The number of samples in the loop portion of BGM.
 */
uint16_t la0_get_bgm_loop_samples(void);

/**
 * `MSX` Return the loop counter value.
 *
 * The loop counter starts at 0 and counts up when the song loops back.
 *
 * In case of overflow (exceeding 255), the counter returns to 0.
 *
 * \return The loop counter value.
 */
uint8_t la0_get_bgm_loop_counter(void);

#endif // LA0_H_
