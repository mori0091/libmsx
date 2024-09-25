;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2024 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file save_slots.s

        .module save_slots

        .globl  get_slot

;------------------------------------------------
        .area   _CODE

;;; Save current slot address of each pages.
;;; The below is assumed ;
;;; page #0 0x0000..0x3fff is MAIN-ROM
;;; page #1 0x4000..0x7fff is cartrige ROM
;;; page #2 0x8000..0xbfff is cartrige ROM
;;; page #3 0xc000..0xffff is MAIN-RAM
;;;
;; \post AF, BC, HL are changed.
save_slots::
        ld      h, #0x00
        call    get_slot
        ld      (_libmsx_initial_slots + #0), a
        ld      h, #0x40
        call    get_slot
        ld      (_libmsx_initial_slots + #1), a
        ld      h, #0x80
        call    get_slot
        ld      (_libmsx_initial_slots + #2), a
        ld      h, #0xc0
        call    get_slot
        ld      (_libmsx_initial_slots + #3), a

;------------------------------------------------
        .area   _DATA

_libmsx_initial_slots::
        .ds      4
