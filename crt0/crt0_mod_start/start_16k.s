;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2023 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file crt0/start/start_16k.s
;;; C startup routine for MSX plain 16KiB ROM cartridge.

        .module start_16k

        .globl  _libmsx___init_intr
        .globl  _main

        .area   _CODE
start::
        HIMEM  = 0xfc4a         ; (2 bytes) Pointer to upper limit address of free area
        ld      sp,(HIMEM)
        ;; initialize RAM
        call    gsinit
        ;; install interrupt routine
        call    _libmsx___init_intr
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../sdcc/device/lib/z80/gsinit.s"
