;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crt0/start/start_megarom.s
;;; C startup routine for MSX w/ ROM mapper and SDCC banked call support.
;;;
;;; \note
;;; It is a common part of crt0 and is intended to be used with one of a ROM
;;; mapper-specific modules such as rom_ascii8.s and rom_ascii16.s.

        .module start_megarom

        .globl get_slot_page1
        .globl set_slot_page2
        .globl rom_init
        .globl set_bank
        .globl  _libmsx___init_intr
        .globl  _main

        .area   _CODE
start::
        HIMEM  = 0xfc4a         ; (2 bytes) Pointer to upper limit address of free area
        ld      sp,(HIMEM)
        call    get_slot_page1
        call    set_slot_page2
        call    rom_init        ; select ROM banks
        call    gsinit          ; initialize RAM
        call    _libmsx___init_intr ; install interrupt routine
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../sdcc/device/lib/z80/gsinit.s"
