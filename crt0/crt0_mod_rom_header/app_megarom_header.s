;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2024 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file crt0/rom_header/app_megarom_header.s
;;;
;;; ROM header for MSX MegaROM image of application, starting at 0x4000.
;;; suggested options: --code-loc 0x4010 --data-loc 0xc000

        .module app_megarom_header
        .globl  init

        .area   _HEADER (ABS)
        ;; ROM header
        .org    0x4000

        .db     0x41
        .db     0x42
        .dw     init
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000

        ;; ---- ordering segments for linker
        .area   _HOME
        .area   _CODE
        .area   _INITIALIZER
        .area   _GSINIT
        .area   _GSFINAL
        ;; ----
        .area   _DATA
        .area   _INITIALIZED
        .area   _BSEG
        .area   _BSS
        .area   _HEAP
        ;; ----
        .area   _BANK1
        .area   _BANK2
        .area   _BANK3
        .area   _BANK4
        .area   _BANK5
        .area   _BANK6
        .area   _BANK7
        .area   _BANK8
        .area   _BANK9
        .area   _BANK10
        .area   _BANK11
        .area   _BANK12
        .area   _BANK13
        .area   _BANK14
        .area   _BANK15
        .area   _BANK16
        .area   _BANK17
        .area   _BANK18
        .area   _BANK19
        .area   _BANK20
        .area   _BANK21
        .area   _BANK22
        .area   _BANK23
        .area   _BANK24
        .area   _BANK25
        .area   _BANK26
        .area   _BANK27
        .area   _BANK28
        .area   _BANK29
        .area   _BANK30
        .area   _BANK31
        .area   _BANK32
        .area   _BANK33
        .area   _BANK34
        .area   _BANK35
        .area   _BANK36
        .area   _BANK37
        .area   _BANK38
        .area   _BANK39
        .area   _BANK40
        .area   _BANK41
        .area   _BANK42
        .area   _BANK43
        .area   _BANK44
        .area   _BANK45
        .area   _BANK46
        .area   _BANK47
        .area   _BANK48
        .area   _BANK49
        .area   _BANK50
        .area   _BANK51
        .area   _BANK52
        .area   _BANK53
        .area   _BANK54
        .area   _BANK55
        .area   _BANK56
        .area   _BANK57
        .area   _BANK58
        .area   _BANK59
        .area   _BANK60
        .area   _BANK61
        .area   _BANK62
        .area   _BANK63
