# DESIGN NOTE : SNDDRV - a sound driver

# Features

- Supported sound chip
  - PSG (AY-3-8910)
- Functionallity
  - NTSC (60Hz), PAL/SECAM (50Hz), auto detect and auto adjust.
  - Available to specify default playing frequency that composer expecting.
  - Run-time speed / repaly frequency control.
  - Stop, Start, Pause, Resume, Auto-Repeat.
  - Playing sound effects (SFX) during playing background music (BGM).
  - Same data format for BGM and SFX.
  - Two type of sound data formats
    - `SND_STREAM`  : Interleaved multi-channel single stream, or
    - `SND_PROGRAM` : Structured music program.
- Expressions
  - Arpeggio
  - Pitch slide
  - Pitch glide
  - Volume
  - Fade in
  - Fade out
  - User defined arpeggio tables
  - User defined pitch bend tables (e.g. for vibrato)
- Instrument (timbre)
  - User defined instrument (timbre) tables
    - Oscillator (Square wave)
    - Software envelope generator
    - Noise generator
    - Tone / Noise mixing
    - Hardware envelope generator (Saw, Triangle, Inv-Saw, Inv-Triangle wave)
    - Use hardware envelope generator as frequency modulator!
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
|   snd_Sound   | ---> (M streamer) --------------> music data stream --------> (M decoder) ---> (engine) ---> (sound chip)
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

    <term> = <identifier>      ; name of a variable
           | <term> <term>     ; sequence of <term>
           | "(" <term> ")"    ; group of <term>
           | <term> "|" <term> ; left-hand <term> or right-hand <term>
           | <term>"?"         ; 0 or 1 <term>
           | <term>"*"         ; 0 or more <term>
           | <term>"+"         ; 1 or more <term>
           | <term>"{"<n>"}"   ; <n> times repetition of <term>
           | <bit>{<bit>}"b"   ; A bit string such as "1001b".
           | "0:k"             ; A k-bit string of all 0's. (k > 0)
           | "1:k"             ; A k-bit string of all 1's. (k > 0)
           | "_:k"             ; A k-bit string of any value. (k > 0)
           | <term>":k"        ; <term> that is a k-bit string (k > 0)
    
    <bit>  = "0"               ; 1 bit zero
           | "1"               ; 1 bit one
           | "_"               ; 1 bit wildcard

# Definition of Music stream and Track stream

The following is the sound data format for **music streams** and **track
streams** written in the notation defined in the previous section.

``` 
; Music stream
m_stream   = ((channel t_chunk)* wait)* EOM

channel    = 1b c:7                  ; c: channel # (0..127)
wait       = 0b w:7                  ; w: wait count [ticks]
EOM        = 1111b 1111b

; Track stream
t_stream   = t_line*

; A line of track
t_line     = t_chunk* EOL
t_chunk    = expressions
           | NoteOff
           | Legato
           | NoteOn

EOL        = 1110b l:4               ; l: number of lines - 1

NoteOff    = 0:8
Legato     = 0b n:7 0:8              ; n: note # (1..127)
NoteOn     = 0b n:7 i:8              ; n: note # (1..127), i: instrument # (1..255)

expresions = 1000b n:4 expr0{n+1}    ; n: number of expressions - 1

expr0      = 0000b x:4               ; reset effect and set volume to 15-x
           | 0001b x:4 y:4 _:4       ; arpeggio 3 notes (+0, +x, +y)
           | 0010b x:4 y:4 z:4       ; arpeggio 4 notes (+0, +x, +y, +z)
           | 0011b x:4 y:4 z:4       ; pitch up   (+xyz for each some ticks)
           | 0100b x:4 y:4 z:4       ; pitch down (-xyz for each some ticks)
           | 0101b x:4 y:4 z:4       ; fast pitch up   (+xyz for each tick)
           | 0110b x:4 y:4 z:4       ; fast pitch down (-xyz for each tick)
           | 0111b x:4 y:4 z:4       ; pitch glide (glide for each xyz+1 ticks)
           | 1000b x:4               ; set volume to x
           | 1001b x:4 y:4 z:4       ; fade in  (volume +1 for each xyz+1 ticks)
           | 1010b x:4 y:4 z:4       ; fade out (volume -1 for each xyz+1 ticks)
           | 1011b x:4 y:4 _:4       ; force the speed of an instrument (1 step for each xy+1 ticks)
           | 1100b x:4 y:4 _:4       ; force the speed of an arpeggio   (1 step for each xy+1 ticks)
           | 1101b x:4 y:4 _:4       ; force the speed of a pitch slide (1 step for each xy+1 ticks)
           | 1110b x:4 y:4 _:4       ; set arpeggio table # to xy (1..255) or turn arpeggio off (xy = 0)
           | 1111b x:4 y:4 _:4       ; set pitch-slide table # to xy (1..255) or turn pitch-slide off (xy = 0)

```

# Instrument (timbre) table

```
; list of instrument tables
i_tables = i_table_addr*

; an instrument table
i_table  = i_header i_body
i_header = AD_part_addr S_part_addr R_part_addr
i_body   = AD_part S_part R_part
AD_part  = i_chunk+
S_part   = i_chunk* EOM
R_part   = i_chunk* EOM

i_stream = i_chunk* EOM
i_chunk  = Ps? N? V
         | Ps? Ph? N? R? H
```

  - V  
    `___vvvv0` volume

  - N  
    `nnnnnt01` noise mix\_tone?

  - H  
    `wwmmT011` waveform modulation retrig.?
    
      - ww (waveform)
          - 00 : Saw (8)
          - 01 : Triangle (10)
          - 10 : Inv Saw (12)
          - 11 : Inv Triangle (14)
      - mm (modulation)
          - 00 : Hard only
          - 01 : Soft to Hard
          - 10 : Hard to Soft
          - 11 : Soft + Hard

  - R  
    `_rrr0111` ratio

  - Ps  
    `_pa01111` pitch? period\_delta?
    
      - if pa == 01 period\_delta:16
      - else if pa == 10 pitch:16
      - else if pa == 11 period\_delta:16 pitch:16
      - else period:16

  - Ph  
    `pa011111` pitch? period\_delta?
    
      - if pa == 01 period\_delta:16
      - else if pa == 10 pitch:16
      - else if pa == 11 period\_delta:16 pitch:16
      - else period:16

  - EOM  
    `11111111`

| OSC    | SW EG   | HW EG   | NG       | \+0        | \+1        | \+2        |
|--------|---------|---------|----------|------------|------------|------------|
| Square | x SW EG |         |          | `___vvvv0` |            |            |
|        | x SW EG |         | \+ Noise | `nnnnn001` | `___vvvv0` |            |
| Square | x SW EG |         | \+ Noise | `nnnnn101` | `___vvvv0` |            |
|        |         | x HW EG |          | `wwmmT011` |            |            |
|        |         | x HW EG |          | `_rrr0111` | `wwmmT011` |            |
|        |         | x HW EG | \+ Noise | `nnnnn_01` | `wwmmT011` |            |
|        |         | x HW EG | \+ Noise | `nnnnn_01` | `_rrr0111` | `wwmmT011` |
| Square |         | x HW EG |          |            | `wwmmT011` |            |
| Square |         | x HW EG |          | `_rrr0111` | `wwmmT011` |            |
| Square |         | x HW EG | \+ Noise | `nnnnn_01` | `wwmmT011` |            |
| Square |         | x HW EG | \+ Noise | `nnnnn_01` | `_rrr0111` | `wwmmT011` |

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
