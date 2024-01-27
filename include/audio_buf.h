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
 * \file audio_buf.h
 * \brief Unified sound-chip buffer of the libmsx audio replayer.
 * \ingroup AUDIO
 */

#ifndef AUDIO_BUF_H_
#define AUDIO_BUF_H_

#include "memfile.h"
#include <stdint.h>

/**
 * \defgroup AUDIO_BUFFER Unified sound-chip buffer of the libmsx audio replayer.
 * \ingroup AUDIO
 * `#include <audio_buf.h>`
 *
 * Unified sound-chip buffer of the libmsx audio replayer.
 *
 * Music decoder implementation shall use these functions.
 *
 * \sa AUDIO_DECODER
 * @{
 */

/**
 * The latest value of each command.
 *
 * \sa audio_buf_get()
 * \sa audio_buf_put()
 */
extern uint8_t audio_buf_cache[256];

/**
 * `MSX` Initialize the buffer for the libmsx audio replayer.
 *
 * \note There is no need to call it explicitly.
 */
void audio_buf_init(void);

/**
 * `MSX` Clear the internal FIFO of the libmsx audio replayer.
 *
 * This function clears all requests in the FIFO without applying the request to
 * each sound chip; the latest value of each command/register recorded in
 * `audio_buf_cache` remains unchanged.
 */
void audio_buf_clear(void);

/**
 * `MSX` Put requests to the internal FIFO of the libmsx audio replayer.
 *
 * **Commands**
 *
 * The request is a pair of command # `cmd` and operand `val`, representing "a
 * request to write the value `val` to the register of the sound chip
 * corresponding to `cmd`".
 *
 * | `cmd`      | explanation                                                    |
 * |------------|----------------------------------------------------------------|
 * | 0x00..0x1f | SCC/SCC+ ch.1 Waveform registers.                              |
 * | 0x20..0x3f | SCC/SCC+ ch.2 Waveform registers.                              |
 * | 0x40..0x5f | SCC/SCC+ ch.3 Waveform registers.                              |
 * | 0x60..0x7f | SCC/SCC+ ch.4 Waveform registers.                              |
 * | 0x80..0x9f | SCC+ ch.5 Waveform registers.                                  |
 * | 0xa0..0xa1 | SCC/SCC+ ch.1 FDR\_LO, FDR\_HI register.                       |
 * | 0xa2..0xa3 | SCC/SCC+ ch.2 FDR\_LO, FDR\_HI register.                       |
 * | 0xa4..0xa5 | SCC/SCC+ ch.3 FDR\_LO, FDR\_HI register.                       |
 * | 0xa6..0xa7 | SCC/SCC+ ch.4 FDR\_LO, FDR\_HI register.                       |
 * | 0xa8..0xa9 | SCC/SCC+ ch.5 FDR\_LO, FDR\_HI register.                       |
 * | 0xaa       | SCC/SCC+ ch.1 Volume register.                                 |
 * | 0xab       | SCC/SCC+ ch.2 Volume register.                                 |
 * | 0xac       | SCC/SCC+ ch.3 Volume register.                                 |
 * | 0xad       | SCC/SCC+ ch.4 Volume register.                                 |
 * | 0xae       | SCC/SCC+ ch.5 Volume register.                                 |
 * | 0xaf       | SCC/SCC+ Channel mask register.                                |
 * | 0xb0..0xb1 | PSG ch.1 FDR\_LO, FDR\_HI register. (R#0..R#1)                 |
 * | 0xb2..0xb3 | PSG ch.2 FDR\_LO, FDR\_HI register. (R#2..R#3)                 |
 * | 0xb4..0xb5 | PSG ch.3 FDR\_LO, FDR\_HI register. (R#4..R#5)                 |
 * | 0xb6       | PSG Noise FDR register. (R#6)                                  |
 * | 0xb7       | PSG Mixer register. (R#7)                                      |
 * | 0xb8       | PSG ch.1 Volume register. (R#8)                                |
 * | 0xb9       | PSG ch.2 Volume register. (R#9)                                |
 * | 0xba       | PSG ch.3 Volume register. (R#10)                               |
 * | 0xbb..0xbc | PSG H/W envelope FDR\_LO, FDR\_HI register. (R#11..R#12)       |
 * | 0xbd       | PSG H/W envelope number register. (R#12)                       |
 * | 0xbe..0xbf | (reserved)                                                     |
 * | 0xc0..0xc7 | OPLL \$00..\$07 register.                                      |
 * | 0xc8..0xcd | (reserved)                                                     |
 * | 0xce       | OPLL \$0e register.                                            |
 * | 0xcf       | (reserved)                                                     |
 * | 0xd0..0xd8 | OPLL \$10..\$18 register.                                      |
 * | 0xd9..0xdf | (reserved)                                                     |
 * | 0xe0..0xe8 | OPLL \$20..\$28 register.                                      |
 * | 0xe9..0xef | (reserved)                                                     |
 * | 0xf0..0xf8 | OPLL \$30..\$38 register.                                      |
 * | 0xf9       | (reserved)                                                     |
 * | 0xfa       | Copy 32 bytes of waveform from `audio_buf_cache` to SCC/SCC+.  |
 * | 0xfb..0xff | (reserved)                                                     |
 *
 * > The request `cmd = 0xfa, val = i` is a special case.
 * >
 * > This means that copying `audio_buf_cache[32*i+0]..audio_buf_cache[32*i+31]`
 * > to the 32-byte waveform register in the SCC/SCC+ ch.`(i+1)`.
 * >
 * > Example usecase of `cmd = 0xfa`:
 * > ~~~ c
 * > // Ex. Request to set to SCC/SCC+ 2nd channel waveform register.
 * > int8_t wave[32] = { ... };   // 32 bytes waveform
 * > uint8_t ch = 1;              // 2nd channel
 * > // At first, copy the waveform to the corresponding area of `audio_buf_cache[]`.
 * > memcpy((void *)&audio_buf_cache[32*ch], (void *)wave, 32);
 * > // Then request `cmd = 0xfa` with `val = ch`.
 * > audio_buf_put(priority, 0xfa, ch);
 * > ~~~
 *
 *
 * **Priority**
 *
 * Each request is prioritized by `priority`:
 * - By calling `audio_buf_play()`, all requests in the FIFO are basically
 *   processed in order.
 * - However, requests in the FIFO with higher priority will take precedence
 *   over requests with lower priority of the same command. (The lower ones, if
 *   any, are simply skipped/ignored)
 *
 * The `priority` is useful to merge requests for background music (BGM), sound
 * effects (SFX), and effectors (EFX).
 *
 * The following priorities are recommended:
 * (`LA0` has adopted this strategy.)
 *
 * | `priority` | recommended usecase                     |
 * |------------|-----------------------------------------|
 * | `0`        | for background music (BGM)              |
 * | `1`        | for sound effects (SFX)                 |
 * | `2..127`   | (reserved)                              |
 * | `128..255` | for effectors (EFX) such as fade-in/out |
 *
 * Or if the decoder can merge requests for BGM and SFX before put them into the
 * FIFO, the following priorities are recommended:
 * (`SNDDRV` has adopted this strategy.)
 *
 * | `priority` | recommended usecase                     |
 * |------------|-----------------------------------------|
 * | `0`        | for merged BGM and SFX                  |
 * | `1`        | unused                                  |
 * | `2..127`   | (reserved)                              |
 * | `128..255` | for effectors (EFX) such as fade-in/out |
 *
 * \param priority Priority (0..255; 0 is the lowest priority)
 * \param cmd      Command # corresponding to the registers of the sound chip.
 * \param val      Value to be set to the register.
 */
void audio_buf_put(uint8_t priority, uint8_t cmd, uint8_t val);

/**
 * `MSX` Return the latest value of the specified command.
 *
 * This is useful to observe the latest value of registers.
 *
 * In particular, decoders for sound effects (SFX) and effectors (EFX) can
 * calculate register values based on the latest values generated by the decoder
 * for background music.
 *
 * For example, to control the main volume, the audio_efx_amp() effector calls
 * this function to get the latest volume value, and then calls audio_buf_put()
 * to put the changed volume value with higher priority into the FIFO.
 *
 * \param cmd      Command # corresponding to the registers of the sound chip.
 * \return The latest value of the command.
 *
 * \sa audio_buf_put()
 */
inline uint8_t audio_buf_get(uint8_t cmd) {
  return audio_buf_cache[cmd];
}

/**
 * `MSX` Put a request into the internal FIFO to restore the cached value.
 *
 * Called by the replayer to restore the latest register value of the BGM at the
 * end of SFX.
 *
 * Note that the SCC/SCC+ waveform registers are not restored. Therefore, if the
 * SFX decoder changes the waveform, the BGM decoder maybe need to explicitly
 * reset the waveform at the end of SFX.
 *
 * \note There is no need to call it explicitly.
 */
void audio_buf_restore(void);

/**
 * `MSX` Process all requests in the internal FIFO of the libmsx audio replayer.
 *
 * To apply the request in the FIFO to each sound chip, audio_play() calls this
 * function.
 *
 * \post The internal FIFO is empty.
 *
 * \sa audio_buf_put()
 *
 * \note There is no need to call it explicitly.
 */
void audio_buf_play(void);

/**
 * `MSX` Stop (Pause) playing sound on PSG, SCC/SCC+, and OPLL.
 *
 * \post Interrupt is disabled.
 *
 * \note There is no need to call it explicitly.
 */
void audio_buf_stop(void);

/** @} */

#endif // AUDIO_BUF_H_
