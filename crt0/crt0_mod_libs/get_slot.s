;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file get_slot.s

        .module get_slot

;; \brief Get slot address of page N.
;;
;; \param HL  an address
;; \retval A  slot
;; \post Interrupts are enabled.
;; \post AF, BC, HL are changed.
        .area   _CODE
get_slot::
        RSLREG = 0x0138         ; Read SLot selector REGister
        EXPTBL = 0xfcc1         ; (4 bytes) Extended flag table of 4 primary slots
        SLTTBL = 0xfcc5         ; (4 bytes) Save area for secondary slot selector registers
        di
        ld      b, h
        ; A = Primary slot of page #1
        call    RSLREG
        call    slot_number_of
        ; A |= Extended flag of the primary slot
        ld      c, a
        ld      hl, #EXPTBL
        add     a, l
        ld      l, a
        ld      a, (hl)
        and     #0x80
        or      c
        ; A |= Secondary slot of page #1
        ld      c, a
        inc     l
        inc     l
        inc     l
        inc     l
        ld      a, (hl)
        call    slot_number_of
        rlca
        rlca
        or      c
        ; return the slot address
        ei
        ret

;; \brief Extract primary of secondary slot number of page N.
;;
;; \param A  value of primary or extended slot selector register
;; \param BC an address (to determine the page #)
;; \retval A primary or secondary slot number.
;; \post AF are changed.
slot_number_of:
        bit     6, b
        jr      Z, 00001$
        rrca
        rrca
00001$:
        bit     7, b
        jr      Z, 00002$
        rrca
        rrca
        rrca
        rrca
00002$:
        and     #0x03
        ret
