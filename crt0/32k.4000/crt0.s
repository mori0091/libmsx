;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/32k.4000/crt0.s
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

        RSLREG = 0x0138         ; Read SLot selector REGister
        ENASLT = 0x0024         ; ENAble SLoT

        EXPTBL = 0xfcc1         ; (4 bytes) Extended flag table of 4 primary slots
        SLTTBL = 0xfcc5         ; (4 bytes) Save area for secondary slot selector registers
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
        ld      sp, (HIMEM)
        call    find_rom_page_2
        call    gsinit
        call    _libmsx___init_intr
        call    _main
_exit::
        rst     0x00

        ;;
;------------------------------------------------
; find_rom_page_2
; original name     : LOCALIZAR_SEGUNDA_PAGINA
; Original author   : Eduardo Robsy Petrus
; Snippet taken from: http://karoshi.auic.es/index.php?topic=117.msg1465
;
; Rutina que localiza la segunda pagina de 16 KB
; de una ROM de 32 KB ubicada en 4000h
; Basada en la rutina de Konami-
; Compatible con carga en RAM
; Compatible con expansores de slots
;------------------------------------------------
; Comprobacion de RAM/ROM

find_rom_page_2::
        ld      hl, #0x4000
        ld      b, (hl)
        xor     a
        ld      (hl), a
        ld      a, (hl)
        or      a
        jr      nz,5$ ; jr nz,@@ROM
        ; El programa esta en RAM - no buscar
        ld      (hl), b
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
        .include        "../../sdcc/device/lib/z80/gsinit.s"
