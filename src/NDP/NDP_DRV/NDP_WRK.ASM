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
;;; \file NDP_WRK.ASM
;;; Workarea of the NDP driver.
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

;=====================================
;コールエントリ／ワークエリア／テーブル
;（NDP.ASM と NDP_DRV.ASM が別途必要）
;=====================================

	;; .area _CODE

	;; JP	NDPINI
	;; JP	MSTART
	;; JP	MSTOP
	;; JP	INTRPT

	;; JP	U_ADR
	;; JP	U_OFF1
	;; JP	U_OFF2
	;; JP	U_OFF3
	;; JP	U_MV
	;; JP	U_MFO
	;; JP	U_MFI
	;; JP	U_SE

	;; JP	CH1OFF
	;; JP	CH2OFF
	;; JP	CH3OFF
	;; JP	MVSET
	;; JP	MFOSET
	;; JP	RDSTAT
	;; JP	RDENDT
	;; JP	RDLOOP
	;; JP	ADRSET
	;; JP	MPLAYF
	;; JP	SEPLAY
	;; JP	VSET

	;; JP	SYSVER
	;; JP	NDPOFF
	;; JP	SETHF

	;; JP	IMAIN

;------	ワークエリア
	.area _DATA

CH1WRK:	;リズムトラック
	.dw	0	;0-1	演奏データ読み込みアドレス
	.db	0	;2	ノート番号
	.db	0	;3
	.db	0	;4	音長カウンタ
	.db	0	;5	次の1バイトも音長かどうか
	.db	0	;6	255ならリズム
	.db	0	;7	レガート
	.db	0	;8
	.db	0	;9
	.db	0	;10	チャンネル有効/無効
	.db	0	;11
	.db	0	;12	レガート遅延フラグ
	NOP		;13
	NOP		;14
	NOP		;15
	.dw	0	;16 17	ソフトエンベロープポインタ
	NOP		;18
	NOP		;19
	NOP		;20
	NOP		;21
	NOP		;22
	NOP		;23
	NOP		;24
	NOP		;25
	NOP		;26
	NOP		;27
	NOP		;28
	NOP		;29
	NOP		;30
	NOP		;31
	.db	0	;32	リピートネスト数
RVOLW:	.ds	26	;33-58	リズム26種類の音量
	NOP		;59
	NOP		;60

CH2WRK:	;通常トラック
	.dw	0	;+0 1	演奏データ読み込みアドレス
	.db	0	;+2	ノート番号
	.db	0	;+3	設定音量 (0=v15, 1=V14, 2=V13 … 15=v0)
	.db	0	;+4	音長カウンタ
	.db	0	;+5	次の1バイトも音長かどうか
	.db	0	;+6	音色番号 (90〜9FHの場合はハードエンベ番号)
	.db	0	;+7	レガート
	.db	0	;+8	サスティン
	.db	0	;+9	Q(クオンタイズ)の値
	.db	0	;+10	チャンネル有効/無効 0=無効 1=有効 2=効果音発音中 3〜255=一時ミュート(フレーム数)
	.db	0	;+11	ピッチエンベ番号 (bit7=ピッチを戻す？ bit6=ピッチ更新する？ bit5=元音程からの相対値？)
	.db	0	;+12	レガート確認用フラグ（+7のレガート設定の値が1音あとに入る）
	.dw	0	;+13 14	発音中の音程（レジスタ値）
	.db	0	;+15	発音中の音量
	.dw	0	;+16 17	ソフトエンベロープポインタ / ハードエンベ音量半減カウンタ
	.db	0	;+18	ソフトエンベロープウェイトカウンタ
	.dw	0	;+19 20	デチューン
	.dw	0	;+21 22	ピッチエンベロープポインタ
	.db	0	;+23	ピッチエンベロープディレイカウンタ
	.db	0	;+24	リリース音量
	.db	0	;+25	リリースカウンタ設定値
	.db	0	;+26	リリースカウンタ
	.dw	0	;+27 28	リリースディレイ用ピッチ
	.db	0	;+29	リリースディレイ用スイッチ兼指定フレーム数
	.db	0	;+30	リリース音量設定
	.db	0	;+31	キーオン時の音程レジスタ更新フラグ
	.db	0	;+32	リピートネスト数
	.db	0	;+33	ポルタメント加算値(小数)
	.db	0	;+34	ポルタメント加算値
	.dw	0	;+35 36	ポルタメント用の現在音程 (13-14に向かって加減算)
	.db	0	;+37	ポルタメント設定値(小数)
	.db	0	;+38	ポルタメント設定値兼スイッチ (0=OFF)
	.db	0	;+39	ポルタメント単発フラグ
	.db	0	;+40	@Q(後ろを削るクオンタイズ)の値
	.db	0	;+41	キーオン直前のノート番号（リリースディレイ用）
	.db	0	;+42	音量インターバル設定値
	.db	0	;+43	ノートエンベ番号
	.dw	0	;+44 45	ノートエンベロープポインタ
	.db	0	;+46	ノートエンベロープウェイトカウンタ
	.db	0	;+47	音量インターバルのカウンタ
	.db	0	;+48	音量インターバルの音量値
	.dw	0	;+49 50	前回のデチューン値
	.db	0	;+51	リリースディレイ用カウンタ
	.db	0	;+52	ゲートタイム (Qから計算してキーオン時に設定)
	.db	0	;+53	リリースディレイスキップ用カウンタ
	.dw	0	;+54 55	ピッチエンベ適用前の音程（レジスタ値）
	.db	0	;+56	音量インターバル到達値
	.db	0	;+57	固定ゲートタイム
	.db	0	;+58	パラメータの退避領域
	.db	0	;+59	リリースディレイ発動フラグ
	.db	0	;+60	*n（ミックスモード）の値
CH3WRK: .ds	WSIZE
CH4WRK: .ds	WSIZE

SE1WRK:	.ds	WSIZE*3

CH1RWK:
	.dw	0	;0-1	リピート1開始アドレス
	.dw	0	;2-3	リピート1終了アドレス
	.db	0	;4	リピート1回数
	.dw	0	;5-6	リピート2開始アドレス
	.dw	0	;7-8	リピート2終了アドレス
	.db	0	;9	リピート2回数
	.dw	0	;10-11	リピート3開始アドレス
	.dw	0	;12-13	リピート3終了アドレス
	.db	0	;14	リピート3回数
	.dw	0	;15-16	リピート4開始アドレス
	.dw	0	;17-18	リピート4終了アドレス
	.db	0	;19	リピート4回数
CH2RWK:	.ds	RWSIZE
CH3RWK:	.ds	RWSIZE
CH4RWK:	.ds	RWSIZE

;->*************** MSTART 初期化時に0クリアする物

FFFLG:	.db	0	;早送りフラグ
ENDTR:	.db	0	;演奏データが終端まで達したトラックのビットが立つ (0000321RB) ※1ループでリセット
ENDTRW:	.db	0	;〃 ※1ループ後もリセットしない
ENDTRR:	.db	0	;〃 リセット時に書き戻す値 (曲開始時に未使用トラックのフラグを立てておいた値)
MCOUNT:	.db	0	;ループ回数
MVOL:	.db	0	;マスター音量 (この値を減算)
MFADE:	.db	0	;フェード設定 (0=フェードしない 1〜255=フェードカウント)
FCOUNT:	.db	0	;フェードカウンタ
LCOUNT:	.db	0	;フェードループカウンタ
SLWPRM:	.db	0	;スロー設定
SLWCNT:	.db	0	;スロー再生用カウンタ
NFREQW:	.db	0	;ノイズ周波数
VISPAN:	.db	0	;音量インターバル周期

RNON:	.db	0	;リズム発音中かどうか (0=発音しない 1=発音終了してピッチを戻す 2=発音中 3=1フレだけ通常トーン)
RSVOL:	.db	0	;リズムを鳴らす前の音量退避
RPITCH:	.dw	0	;リズムを鳴らす前の音程退避

HENVSW:	.db	0	;ハードエンベ番号

CLREND:

;<-***************

RVWRK:	.db	0	;リズム音量反映用
RITRK:	.db	1	;割り込まれトラック (1=CH.C 2=CH.B 3=CH.A)
RVREG:	.db	10	;リズムトラックの音量レジスタ番号
RPREG:	.db	5	;リズムトラックの周波数レジスタ番号(上位)
RHENV:	.db	0	;リズム内でSコマンドが実行されていなければ0になる

;; OLDTH:	.ds	5	;旧タイマ割り込みフック
;; HKFLG:	.db	0	;フックにドライバを接続済みかどうか

STATS:	.db	0	;演奏状態 (0:停止 1:再生)

FINOUT:	.db	0	;0=フェードアウト 1=フェードイン

MIXNMH:	.db	1	;ハードエンベ使用時の*n（ミックスモード）の値

MIXWRK:	.db	0b10111000	;10NNNTTT 0=ON/1=OFF
MIXWRS:	.db	0b10111000	;効果音用ミックスワーク

HENVPW:	.dw	1024	;ハードエンベ周期

SEMODE:	.db	0	;演奏モード (0:BGM 1:SE) ※割り込み内で設定
SEBAKT:	.ds	CHNUM	;効果音に割り込まれる側のトラック有効フラグをここに退避
SEBAKR:	.db	0	;リズムのキーオフ時にSE終了時のレジスタリセットを走らせるかどうか
SECNT:	.db	0	;効果音のトラックカウンタ（鳴っている最中の効果音のトラック数）

VADTBL:	.ds	32	;音量エンベアドレステーブル
RADTBL:	.ds	64	;リズム音色アドレステーブル
PADTBL:	.ds	32	;ピッチエンベアドレステーブル
NADTBL:	.ds	32	;ノートエンベアドレステーブル

BGMADR:	.dw	0x04000	;曲データ先頭アドレス

;------	音程テーブル
        .area   _INITIALIZED
PTABLE:
	.ds	SIZE_OF_PTABLE
O4C = PTABLE + 3 * 12 * 2

        .area   _INITIALIZER
__xinit_PTABLE:
;		c      c+     d      d+     e      f      f+     g      g+     a      a+     b
	.dw	0x0D5D,0x0C9C,0x0BE7,0x0B3C,0x0A9B,0x0A02,0x0973,0x08EB,0x086B,0x07F2,0x0780,0x0714	;O1
	.dw	0x06AF,0x064E,0x05F4,0x059E,0x054E,0x0501,0x04BA,0x0476,0x0436,0x03F9,0x03C0,0x038A	;O2
	.dw	0x0357,0x0327,0x02FA,0x02CF,0x02A7,0x0281,0x025D,0x023B,0x021B,0x01FD,0x01E0,0x01C5	;O3
	.dw	0x01AC,0x0194,0x017D,0x0168,0x0153,0x0140,0x012E,0x011D,0x010D,0x00FE,0x00F0,0x00E3	;O4
	.dw	0x00D6,0x00CA,0x00BE,0x00B4,0x00AA,0x00A0,0x0097,0x008F,0x0087,0x007F,0x0078,0x0071	;O5
	.dw	0x006B,0x0065,0x005F,0x005A,0x0055,0x0050,0x004C,0x0047,0x0043,0x0040,0x003C,0x0039	;O6
	.dw	0x0035,0x0032,0x0030,0x002D,0x002A,0x0028,0x0026,0x0024,0x0022,0x0020,0x001E,0x001C	;O7
	.dw	0x001B,0x0019,0x0018,0x0016,0x0015,0x0014,0x0013,0x0012,0x0011,0x0010,0x000F,0x000E	;O8
	SIZE_OF_PTABLE = . - __xinit_PTABLE
