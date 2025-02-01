;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/rom_mapper/rom_ascii8.s
;;;
;;; Copyright (c) 2021-2025 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

        .module crt0

        ;; ROM Mapper registers (ROM type: ASCII8)
        ROM_MAPPER_REGISTER_0 = 0x6000
        ROM_MAPPER_REGISTER_1 = 0x6800
        ROM_MAPPER_REGISTER_2 = 0x7000
        ROM_MAPPER_REGISTER_3 = 0x7800

        .area   _CODE
rom_init::
        xor     a
        ld      (ROM_MAPPER_REGISTER_0), a ; segment #0
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a ; segment #1
        inc     a
        ld      (ROM_MAPPER_REGISTER_2), a ; segment #2 (bank #1.0)
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a ; segment #3 (bank #1.1)
        ret

;------------------------------------------------
        .area   _INITIALIZED
set_bank::
set_bank_p2::
        .ds     SIZE_OF_set_bank_p2

get_bank::
get_bank_p2::
        .ds     SIZE_OF_get_bank_p2

cur_bank_p2:
        .ds     SIZE_OF_cur_bank_p2

;------------------------------------------------
        .area   _INITIALIZER
__xinit_set_bank_p2:
        ld      (cur_bank_p2), a
        ;; two 8KiB pages per one bank
        add     a, a
        ld      (ROM_MAPPER_REGISTER_2), a ; segment #2n+0 (lower 8KiB of bank #n)
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a ; segment #2n+1 (upper 8KiB of bank #n)
        ret
        SIZE_OF_set_bank_p2 = . - __xinit_set_bank_p2

__xinit_get_bank_p2:
        ld      a, (cur_bank_p2)
        ret
        SIZE_OF_get_bank_p2 = . - __xinit_get_bank_p2

__xinit_cur_bank_p2:
        .db     1
        SIZE_OF_cur_bank_p2 = . - __xinit_cur_bank_p2
