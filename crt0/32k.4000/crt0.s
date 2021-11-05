;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; crt0 for MSX ROM of 32KB starting at 0x4000
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
        call    find_rom_page_2
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
	ld hl, #0x4000
	ld b, (hl)
	xor a
	ld (hl), a
	ld a, (hl)
	or a
	jr nz,5$ ; jr nz,@@ROM
	; El programa esta en RAM - no buscar
	ld (hl),b
	ret
5$: ; ----------- @@ROM:
	di
	; Slot primario
	call #0x0138 ; call RSLREG
	rrca
	rrca
	and #0x03
	; Slot secundario
	ld c, a
	ld hl, #0xfcc1
	add a, l
	ld l, a
	ld a, (hl)
	and #0x80
	or c
	ld c, a
	inc l
	inc l
	inc l
	inc l
	ld a, (hl)
	; Definir el identificador de slot
	and #0x0c
	or c
	ld h, #0x80
	; Habilitar permanentemente
	call #0x0024 ; call ENASLT
	ei
	ret
;------------------------------------------------
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
