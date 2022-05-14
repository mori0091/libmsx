;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/rom_mapper/crt0.s
;;;
;;; Copyright (c) 2021 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; crt0 for MSX ROM of 32KB starting at 0x4000
;;; suggested options: --code-loc 0x4010 --data-loc 0xc000
;;; `main` should be `void main(void)`
;;; `return` from `main` causes soft reset.

        .module crt0

        .globl  _main
        .globl  _exit
        .globl  _libmsx___init_intr
        .globl rom_init
        .globl set_bank

        HIMEM  = 0xfc4a         ; (2 bytes) Pointer to upper limit address of free area

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
        .area   _DATA
        .area   _INITIALIZED
        .area   _BSEG
        .area   _BSS
        .area   _HEAP
        ;; ----

        .area   _CODE
init:
        ld      sp,(HIMEM)
        call    find_rom_page_2 ; select ROM slots
        call    rom_init        ; select ROM banks
        call    gsinit          ; initialize RAM
        call    _libmsx___init_intr ; install interrupt routine
        call    _main
_exit::
        rst     0x00
1$:
        halt
        jr      1$

;------------------------------------------------
        RSLREG = 0x0138         ; Read SLot selector REGister
        ENASLT = 0x0024         ; ENAble SLoT

        EXPTBL = 0xfcc1         ; (4 bytes) Extended flag table of 4 primary slots
        SLTTBL = 0xfcc5         ; (4 bytes) Save area for secondary slot selector registers

        .area   _CODE

find_rom_page_2::
        ld      hl, #0x4000
        ld      b, (hl)
        xor     a
        ld      (hl), a
        ld      a, (hl)
        or      a
        jr      nz,5$ ; jr nz,@@ROM
        ; El programa esta en RAM - no buscar
        ld      (hl),b
        ret
5$: ; ----------- @@ROM:
        di
        ; Slot primario
        call    RSLREG
        rrca
        rrca
        and     #0x03
        ; Slot secundario
        ld      c, a
        ld      hl, #EXPTBL
        add     a, l
        ld      l, a
        ld      a, (hl)
        and     #0x80
        or      c
        ld      c, a
        inc     l
        inc     l
        inc     l
        inc     l
        ld      a, (hl)
        ; Definir el identificador de slot
        and     #0x0c
        or      c
        ld      h, #0x80
        ; Habilitar permanentemente
        call    ENASLT
        ei
        ret

;------------------------------------------------
        ;; void rom_banked_jump(uint8_t a)
        ;; Trampoline code for banked jump
        ;; (jump to the start address of bank a)
        .area   _INITIALIZER
rom_bjump:
        push    af
        call    _libmsx___purge_intr
        pop     af
        call    set_bank
        ld      hl, (0x4002)
        ex      (sp), hl
        ret
        l_rom_bjump = (. - rom_bjump)

        .area   _INITIALIZED
_rom_banked_jump::
        .ds     l_rom_bjump

;------------------------------------------------
        .include        "../../sdcc/device/lib/z80/gsinit.s"
        .include        "../../sdcc/device/lib/z80/__sdcc_bcall.s"
        ;; .include        "../rom_mapper/rom_ascii8.s"
