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
 * \file slot_bcmp.c
 */

#include "slot.h"

#include "bios.h"

/**
 * Compare the byte sequence to the one present at the given address in the given slot.
 *
 * \param slot,&nbsp;addr  a pair of slot address and address, of the one compared to.
 * \param s                pointer to the byte sequence.
 * \param len              length of the byte sequence.
 *
 * \return 0 if two byte sequence is same (or len was 0), otherwise non-zero value.
 */
int slot_bcmp(uint8_t slot, const void * addr, const void * s, size_t len) {
  const char * p = (const char *)addr;
  const char * q = (const char *)s;
  while (len--) {
    uint8_t c = msx_RDSLT(slot, (void *)p);
    __asm__("ei");
    if (c != *q) {
      return -1;
    }
    p++;
    q++;
  }
  return 0;
}
