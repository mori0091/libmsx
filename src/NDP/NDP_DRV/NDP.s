; -*- coding: utf-8-unix; tab-width: 8; mode: asm -*-

; --------------------------------------------------------------
;;; Copyright (c) 2021-2025 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx
; --------------------------------------------------------------
;;; \file NDP.s
;;; NDP driver and its entry points for C.
;;;
;;; Copyright (c) 2025 naruto2413
;;; Modifications copyright (c) 2025 Daishi Mori (mori0091)
;;;
;;; This software (NDP - PSG Driver for MSX) was originally programmed and
;;; provided by naruto2413 and later modified for libmsx by Daishi Mori
;;; (mori0091).
;;;
;;; The original NDP driver for MSX and MML editor/compiler for Windows are
;;; distributed at the following site by naruto2413.
;;;
;;; NDP - PSG Driver for MSX
;;; https://ndp.squares.net/web/
; --------------------------------------------------------------

;[NDP] - PSG Music Driver for MSX - Programmed by naruto2413

	.module NDP

	HTIMI = 0x0FD9F	;タイマ割り込みフック
	WSIZE = 61	;各トラックのワークエリアのサイズ
	RWSIZE = 5*4	;各トラックのリピート用ワークエリアのサイズ (1ネストあたりのサイズ*ネスト数)F
	CHNUM = 4	;使用チャンネル数

	.include "./NDP_DRV.ASM"
	.include "./NDP_WRK.ASM"

;; Export entry points with prefix for C

	.area	_CODE

	;; _NDP_NDPINI == NDPINI	;ドライバ初期化 (タイマ割り込みフックにドライバを接続)
	_NDP_MSTART == MSTART	;演奏開始 (ワークエリアをゼロクリアし演奏開始。フックは操作しない)
	_NDP_MSTOP  == MSTOP	;演奏停止 (演奏停止してPSGレジスタを初期化。フックは操作しない)
	_NDP_INTRPT == INTRPT	;割り込み処理 (フックに接続せずに自前で割り込みルーチンを呼ぶ際に使用)
	;; _NDP_U_ADR  == U_ADR	;(※)曲データ開始アドレス設定 (引数で指定。デフォルトの04000Hなら設定不要)
	;; _NDP_U_OFF1 == U_OFF1	;(※)Ch.A 一時ミュート (ミュートするフレーム数を引数で指定 [2～255])
	;; _NDP_U_OFF2 == U_OFF2	;(※)Ch.B 〃
	;; _NDP_U_OFF3 == U_OFF3	;(※)Ch.C 〃
	;; _NDP_U_MV   == U_MV	;(※)マスター音量オフセット (引数で渡した値を音量値から減算する)
	;; _NDP_U_MFO  == U_MFO	;(※)フェードアウト (引数で渡したフレーム数ずつフェードアウト)
	;; _NDP_U_MFI  == U_MFI	;(※)フェードインしながら演奏開始 (引数で渡したフレーム数ずつフェードイン)
	;; _NDP_U_SE   == U_SE	;(※)効果音発音 (効果音データのアドレスを引数で指定)
	_NDP_CH1OFF == CH1OFF	;Ch.A 一時ミュート (D←ミュートするフレーム数 [2～255])
	_NDP_CH2OFF == CH2OFF	;Ch.B 〃
	_NDP_CH3OFF == CH3OFF	;Ch.C 〃
	_NDP_MVSET  == MVSET	;マスター音量オフセット (A←音量値から減算する値)
	_NDP_MFOSET == MFOSET	;フェードアウト (A←各段階のフレーム数)
	_NDP_RDSTAT == RDSTAT	;演奏状態→A (0:停止中 1:演奏中)
	_NDP_RDENDT == RDENDT	;各トラックの演奏が終端まで達したかどうか→A (0000321Rのビットフィールド)
	_NDP_RDLOOP == RDLOOP	;曲がループした回数→A (曲データの+0BHを見て無限ループしない曲なら演奏終了時に255が返る)
	_NDP_ADRSET == ADRSET	;曲データ開始アドレス設定 (DE←アドレス。デフォルトの04000Hなら設定不要)
	_NDP_MPLAYF == MPLAYF	;フェードインしながら演奏開始 (A←各段階のフレーム数)
	_NDP_SEPLAY == SEPLAY	;効果音発音 (DE←効果音データのアドレス)
	_NDP_VSET   == VSET	;音色データをドライバに反映 (HL←音色定義データのアドレス)
	_NDP_SYSVER == SYSVER	;ドライバのバージョン番号→HL (L=小数点以下、H=整数部。整数部が0なら0.9として扱う)
	;; _NDP_NDPOFF == NDPOFF	;ドライバ終了 (タイマ割り込みフックからドライバを切り離して発音停止)
	;; _NDP_SETHF  == SETHF	;タイマ割り込みフック接続フラグ←A
	;; _NDP_KSSPLY == KSSPLY	;退避したH.TIMIをRETで潰して演奏開始 (KSSプレイヤー用)

;; C APIs for libmsx
	_NDP_fadeout		== _NDP_MFOSET ; void NDP_fadeout(uint8_t wait)
	_NDP_fadein		== _NDP_MPLAYF ; void NDP_fadein(uint8_t wait)
	_NDP_is_playing		== _NDP_RDSTAT ; bool NDP_is_playing(void)
	_NDP_get_track_status	== _NDP_RDENDT ; uint8_t NDP_get_track_status(void)
	_NDP_get_loop_counter	== _NDP_RDLOOP ; uint8_t NDP_get_loop_counter(void)
