;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/rom_mapper/rom_ascii8.s
;;;
;;; Copyright (c) 2021 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

        .globl  _libmsx___purge_intr

        ;; ROM Mapper registers (ROM type: ASCII8)
        ROM_MAPPER_REGISTER_0 = 0x6000
        ROM_MAPPER_REGISTER_1 = 0x6800
        ROM_MAPPER_REGISTER_2 = 0x7000
        ROM_MAPPER_REGISTER_3 = 0x7800

        .area   _CODE
rom_init::
        xor     a
        ld      (ROM_MAPPER_REGISTER_0), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_2), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a
        ret

;------------------------------------------------
        .area   _INITIALIZER
setbank:
        ld      (cur_bank), a
        ;; four 8KiB pages per one bank
        add     a, a
        add     a, a
        ld      (ROM_MAPPER_REGISTER_0), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_2), a
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a
        ret
        l_setbank = (. - setbank)

        .area   _INITIALIZED
set_bank::
        .ds     l_setbank

;------------------------------------------------
        .area   _INITIALIZER
getbank:
        ld      a, (cur_bank)
        ret
        l_getbank = (. - getbank)

        .area   _INITIALIZED
get_bank::
        .ds     l_getbank

;------------------------------------------------
        .area   _DATA
cur_bank:
        .ds     1
