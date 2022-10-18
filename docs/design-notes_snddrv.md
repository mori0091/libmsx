# DESIGN NOTE : SNDDRV - a sound driver

# Features

- Supported sound chip
  - PSG (AY-3-8910)
- Functionallity
  - NTSC (60Hz), PAL/SECAM (50Hz), auto detect and auto adjust.
  - Available to specify default playing frequency that the sound data author expecting.
  - Run-time replay speed/frequency control.
  - Stop, Start, Pause, Resume, Auto-Repeat.
  - Playing sound effects (SFX) during playing background music (BGM).
  - Same data format for BGM and SFX.
- Expressions
  - Arpeggio
  - Pitch slide
  - Pitch glide
  - Volume
  - Volume fade in
  - Volume fade out
  - User defined arpeggio tables
  - User defined pitch bend tables (e.g. for vibrato)
- Instrument (timbre)
  - User defined instrument (timbre) tables
    - Oscillator (Square wave)
    - Software amplitude envelope generator
    - Noise generator
    - Tone / Noise mixing
    - Hardware amplitude envelope generator (Saw, Triangle, Inv-Saw, Inv-Triangle wave)
    - Use hardware envelope generator as **frequency modulator**!
    - Arpeggio / Pitch-bend effect
    - Period (wave cycle) modification

# Internal of SNDDRV

SNDDRV's sound processing pipeline is consisting of 3 part of components: some
**streamers**, some **decorders**, and **engine**.

  - streamer  
    The streamer converts multi-channel tracks, expression tables, and
    instrument tables into octet streams consisting of interleaved
    time-series data.
  - decoder  
    The decoder decodes octet streams into a set of values for each
    tick.
  - engine  
    The engine, for each tick, takes a set of values from decoder,
    converts / synthesis them into a set of register values, and set to
    registers of sound chip.

<!-- end list -->

```
+---------------+
| <<container>> |
|  snd_Program  | ---> (M streamer) --------------> sound data stream --------> (M decoder) ---> (engine) ---> (sound chip)
+---------------+         ↑    |                                                     |               ↑           i.e. PSG
                          `----' sequence control   (o) <------- note on/off --------'               |
                                                        (i) <--- I program change ---'               |
                                                        (a) <--- A program change ---'               |
                                                        (p) <--- P program change ---'               |
+---------------+                                                                                    |
| <<container>> |                                                                                    |
|    i_table    | ---> (I streamer) --------------> instrument data stream ---> (I decoder) ---------'
+---------------+         ↑    |                                                                     |
                          `----' sequence control                                                    |
                          `----- note on/off ------ (o)                                              |
                          `----- I program change ----- (i)                                          |
+---------------+                                                                                    |
| <<container>> |                                                                                    |
|    a_table    | ---> (A streamer) --------------> arpeggio data stream -----> (A decoder) ---------'
+---------------+         ↑    |                                                                     |
                          `----' sequence control                                                    |
                          `----- note on/off ------ (o)                                              |
                          `----- A program change ----- (a)                                          |
+---------------+                                                                                    |
| <<container>> |                                                                                    |
|    p_table    | ---> (P streamer) --------------> pitch bend data stream ---> (P decoder) ---------'
+---------------+         ↑    |
                          `----' sequence control
                          `----- note on/off ------ (o)
                          `----- P program change ----- (p)
```

# Sound data format notation

Here is the notation for defining our sound data format.  
This notation itself is described in BNF notation.

    <term> := <identifier>        ; name of a variable
            | <term> <term>       ; sequence of <term>
            | '(' <term> ')'      ; group of <term>
            | <term> '|' <term>   ; left-hand <term> or right-hand <term>
            | <term> '?'          ; 0 or 1 <term>
            | <term> '*'          ; 0 or more <term>
            | <term> '+'          ; 1 or more <term>
            | <term> '{' <n> '}'  ; <n> times repetition of <term>
            | <bits> 'b'          ; A binary such as "1001b"
            | <term> ':' <k>      ; <term> that is a <k> bit binary
    
    <bits> := <bit>
            | <bit> <bits>

    <bit>  := '0'                 ; 1 bit zero
            | '1'                 ; 1 bit one
            | '_'                 ; 1 bit wildcard

# Definition of Sound data

## Definition of SpeedTrack stream

## Definition of EventTrack stream

## Definition of Track stream

The following is the sound data format for **track streams** written in the
notation defined in the previous section.

### Track stream
```
; Track stream
t_stream   := t_line*

; A line of track
t_line     := t_chunk* EOL

; end mark of line, and number of lines to be skipped.
EOL        := 110b l:5            ; l: number of lines to be skipped (0..31)
```

### Chunk of Track stream
```
; A chunk of track stream
t_chunk    := RST                 ; stops the sound of the instrument.
            | Note                ; a note command
            | Exprs               ; expressions

RST        := 1110b _:4           ; reset effect and set volume to 0

Note       := NoteOff             ; key off
            | NoteOn              ; key on
            | Legato              ; switch note w/o key on

   NoteOff := 0:8
   NoteOn  := 0b n:7 i:8          ; n: note # (1..127), i: instrument # (1..255)
   Legato  := 0b n:7 0:8          ; n: note # (1..127)

Exprs      := 10_0b n:4 expr{n+1} ; n: number of expressions - 1

   expr    := 0000b x:4           ; reset effect and set volume to 15-x
            | 0001b x:4 y:4 _:4   ; arpeggio 3 notes (note +0, +x, +y)
            | 0010b x:4 y:4 z:4   ; arpeggio 4 notes (note +0, +x, +y, +z)
            | 0011b x:4 y:4 z:4   ; pitch up         (period -xyz/256 for each ticks)
            | 0100b x:4 y:4 z:4   ; pitch down       (period +xyz/256 for each ticks)
            | 0101b x:4 y:4 z:4   ; fast pitch up    (period -xyz/16 for each tick)
            | 0110b x:4 y:4 z:4   ; fast pitch down  (period +xyz/16 for each tick)
            | 0111b x:4 y:4 z:4   ; pitch glide      (pitch ±1 for each xyz+1 ticks)
            | 1000b x:4           ; set volume to x  (volume ← x)
            | 1001b x:4 y:4 z:4   ; fade in          (volume +xyz/128 for each ticks)
            | 1010b x:4 y:4 z:4   ; fade out         (volume -xyz/128 for each ticks)
            | 1011b x:4 y:4 _:4   ; force the speed of an instrument (1 step for each xy+1 ticks)
            | 1100b x:4 y:4 _:4   ; force the speed of an arpeggio   (1 step for each xy+1 ticks)
            | 1101b x:4 y:4 _:4   ; force the speed of a pitch bend  (1 step for each xy+1 ticks)
            | 1110b x:4 y:4 _:4   ; set arpeggio table # to xy (1..255) or turn arpeggio off (xy = 0)
            | 1111b x:4 y:4 _:4   ; set pitch bend table # to xy (1..255) or turn pitch bend off (xy = 0)
```

## Definition of Pattern table

# Instrument (timbre) table

```
; list of instrument tables
i_tables := address_of_i_table+

; an instrument table
i_table  := address_of_AD_part
            address_of_S_part
            address_of_R_part
AD_part  := i_chunk* EOM          ; Attack to Decay phase
S_part   := i_chunk* EOM          ; Sustain phase
R_part   := i_chunk* EOM          ; Release phase

i_chunk  := N? Ps? (V | (R? Ph? H))
EOM      := 11111111b

; Volume
V  := _:3 v:4 0b                  ; (___vvvv0) v = volume (0..15)

; Noise generator and Mixer
N  := n:5 t:1 01b                 ; (nnnnnt01) n = noise (0..31), t = mix_tone?

; Hardware envelope and Modulation type
H  := w:3 m:2 011b                ; (wwmmT011) w = waveform, m = modulation
  ; Waveform of hardware envelope
  w := 000b ; ( 8) ◣◣◣◣ Saw tooth
    |  001b ; ( 9) ◣___
    |  010b ; (10) ◣◢◣◢ Triangle
    |  011b ; (11) ◣■■■
    |  100b ; (12) ◢◢◢◢ Inverse Saw tooth
    |  101b ; (13) ◢■■■
    |  110b ; (14) ◢◣◢◣ Inverse Triangle
    |  111b ; (15) ◢___
  ; Modulation type
  m := 00b ; Hard only
    |  01b ; Soft to Hard
    |  10b ; Hard to Soft
    |  11b ; Soft and Hard

; Coefficients used in wavelength calculations for square wave and hardware envelope modulation.
R  := r:3 T:1 0111b                           ; (rrrT0111) r = ratio (0..7) T = retrigger?

Ps := _00b 01111b period:16                   ; (_pa01111 period)
   |  _01b 01111b period_delta:16             ; (_pa01111 period_delta)
   |  _10b 01111b pitch:16                    ; (_pa01111 pitch)
   |  _11b 01111b period_delta:16 pitch:16    ; (_pa01111 period_delta pitch)

Ph := 00b 011111b period:16                   ; (pa011111 period)
   |  01b 011111b period_delta:16             ; (pa011111 period_delta)
   |  10b 011111b pitch:16                    ; (pa011111 pitch)
   |  11b 011111b period_delta:16 pitch:16    ; (pa011111 period_delta pitch)
```

| OSC    | SW EG   | HW EG   | NG       | \+0        | \+1        | \+2        |
|--------|---------|---------|----------|------------|------------|------------|
| Square | x SW EG |         |          | `___vvvv0` |            |            |
|        | x SW EG |         | \+ Noise | `nnnnn001` | `___vvvv0` |            |
| Square | x SW EG |         | \+ Noise | `nnnnn101` | `___vvvv0` |            |
|        |         | x HW EG |          | `wwwmm011` |            |            |
|        |         | x HW EG |          | `rrrT0111` | `wwwmm011` |            |
|        |         | x HW EG | \+ Noise | `nnnnn101` | `wwwmm011` |            |
|        |         | x HW EG | \+ Noise | `nnnnn101` | `rrrT0111` | `wwwmm011` |
| Square |         | x HW EG |          |            | `wwwmm011` |            |
| Square |         | x HW EG |          | `rrrT0111` | `wwwmm011` |            |
| Square |         | x HW EG | \+ Noise | `nnnnn101` | `wwwmm011` |            |
| Square |         | x HW EG | \+ Noise | `nnnnn101` | `rrrT0111` | `wwwmm011` |

- OSC  
  Oscillator  
  (Square wave generator / Tone generator)
- SW EG  
  Software Envelope Generator  
  (Volume)
- HW EG  
  Hardware Envelope Generaor  
  (Saw, Triangle, Inv-Saw, or Inv-Triangle wave generator)
- NG  
  Noise Generator
