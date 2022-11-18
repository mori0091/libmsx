;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file get_slot_page1.s

        .module get_slot_page1

;; \brief Get slot address of page #1.
;;
;; \retval A  slot
;; \post Interrupts are enabled.
        .area   _CODE
get_slot_page1::
        RSLREG = 0x0138         ; Read SLot selector REGister
        EXPTBL = 0xfcc1         ; (4 bytes) Extended flag table of 4 primary slots
        SLTTBL = 0xfcc5         ; (4 bytes) Save area for secondary slot selector registers
        di
        ; A = Primary slot of page #1
        call    RSLREG
        rrca
        rrca
        and     #0x03
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
        and     #0x0c
        or      c
        ; return the slot address
        ei
        ret
