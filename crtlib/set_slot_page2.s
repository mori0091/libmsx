;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crtlib/set_slot_page2.s

        .module set_slot_page2

;; \brief Switch slot of page #2 to the given slot.
;;
;; \param A  slot
;; \post Interrupts are enabled.
        .area   _CODE
set_slot_page2::
        ENASLT = 0x0024         ; ENAble SLoT
        ld      h, #0x80
        call    ENASLT
        ei
        ret
