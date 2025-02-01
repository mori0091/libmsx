;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2025 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file crt0/start/start_32k.s
;;; C startup routine for MSX plain 32KiB ROM cartridge.

        .module start_32k

        .globl  get_slot
        .globl  save_slots
        .globl  _libmsx___init_intr
        .globl  _main

        .area   _CODE
start::
        ENASLT = 0x0024         ; ENAble SLoT
        HIMEM  = 0xfc4a         ; (2 bytes) Pointer to upper limit address of free area
        ld      sp,(HIMEM)
        ;; get slot of page 1
        ld      h, #0x40
        call    get_slot
        ;; set slot of page 2
        ld      h, #0x80
        call    ENASLT
        ei
        ;; initialize RAM
        call    gsinit
        ;; install interrupt routine
        call    _libmsx___init_intr
        ;; save initial slots
        call    save_slots
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../sdcc/device/lib/z80/gsinit.s"
