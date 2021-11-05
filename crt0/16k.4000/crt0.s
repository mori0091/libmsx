;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; crt0 for MSX ROM of 16KB starting at 0x4000
;;; suggested options: --code-loc 0x4020 --data-loc 0xc000
;;; `main` should be `void main(void)`
;;; `return` from `main` causes soft reset.

        .module crt0
        .globl  _main
        .globl  _exit
        .globl  _libmsx___init_intr

        HIMEM  = 0xfc4a
        H_STKE = 0xfeda

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

init:
init2:
        ld      sp,(HIMEM)
        call    gsinit
        call    _libmsx___init_intr
        call    _main
        jp      _exit

        ;; ---- ordering segments for linker
        .area   _HOME
        .area   _CODE
        .area   _INITIALIZER
        .area   _GSINIT
	.area   _GSFINAL
	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP
        ;; ----

	.area   _CODE
_exit::
        rst     0x00
1$:
        halt
        jr      1$

        ;;
        .area   _GSINIT
gsinit::
        ;; initialize global variables that have no initializer
        ld      bc, #l__DATA
        ld      a, b
        or      a, c
        jr      Z, zeroed_data
        ld      hl, #s__DATA
        ld      (hl), #0x00
        dec     bc
        ld      a, b
        or      a, c
        jr      Z, zeroed_data
        ld      e, l
        ld      d, h
        inc     de
        ldir
zeroed_data:
        ;; initialize global variables that have explicit initializer
	ld	bc, #l__INITIALIZER
	ld	a, b
	or	a, c
	jr	Z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir
gsinit_next:
	.area   _GSFINAL
	ret
