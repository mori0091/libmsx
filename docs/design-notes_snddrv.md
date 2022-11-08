# DESIGN NOTE : SNDDRV - a sound driver

# About

SNDDRV is a new PSG sound driver inspired by [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).

SNDDRV covers almost feature of replayer of the Arkos Tracker 2.

SNDDRV and libmsx's build system make it easy to integrate Arkos Tracker 2 song
file (.aks file) into your application.

Let's make your music with Arkos Tracker 2. Place its .aks files in your
application's repository, and build together with your application code that
uses SNDDRV API.

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
  - User defined arpeggio / pitch-bend tables
  - User defined period / wavelength modification tables (e.g. for vibrato)
- Instrument (timbre)
  - User defined instrument (timbre) tables
    - Software amplitude envelope generator
    - Noise generator
    - Tone / Noise mixing
    - Hardware amplitude envelope generator (Saw, Triangle, Inv-Saw, Inv-Triangle wave)
    - Use hardware envelope generator as **frequency modulator**!
    - Arpeggio / Pitch-bend
    - Period / wavelength modification
- Ease to integration
  - A support tool `aks2c.sh` translates Arkos Tracker 2 song file (.aks) to C source file.
  - The `libmsx`'s build system (makefiles) can automatically translates .aks
    files into C source files and built them into your application. It automates
    the following workflow.
    1. Translate .aks files into C source files.
    2. Compile translated C source files, all other C and ASM source files.
    3. Build all of them into your application.

# How to integrate Arkos Tracker 2 .aks files into your application

See also [snddrv_demo](https://github.com/mori0091/libmsx/tree/main/sample/snddrv_demo) the sample project.

# Internal of SNDDRV

The following sections shows the summary of internal design and specification of SNDDRV.

Please note that this information is for reference only and need not be a concern.

## SNDDRV's sound processing pipiline

SNDDRV's sound processing pipeline is consisting of 3 part of components: some
**streamers**, some **decorders**, and **engine**.

- streamer  
  A streamer streams instructions in a track, arpeggio/pitch-bend table, period
  table, or instrument table as an octet stream consisting of time-series data.
- decoder  
  A decoder decodes octet streams into a set of values for each tick.
- engine  
  The engine, for each tick, takes a set of values from decoder, converts /
  synthesis them into a set of register values, and set to registers of sound
  chip.

Sound processing pipeline is configured for each channel of sound chip.

PSG (AY-3-8910) has 3 sound channels in total, so the SNDDRV configures and
controls 3 sound processing pipelines for PSG.

An overview of SNDDRV's sound processing pipeline is shown in the figure below.

```
+----------------+
| <<container>>  |
|   snd_Track    | ---> (T streamer) --------------> track data stream --------> (T decoder) ---> (engine) ---> (sound chip)
+----------------+         ↑    |                                                     |               ↑           i.e. PSG
                           `----' sequence control   (o) <------- note on/off --------'               |
                                                         (i) <--- I program change ---'               |
                                                         (a) <--- A program change ---'               |
                                                         (p) <--- P program change ---'               |
+----------------+                                                                                    |
| <<container>>  |                                                                                    |
| snd_Instrument | ---> (I streamer) --------------> instrument data stream ---> (I decoder) ---------'
+----------------+         ↑    |                                                                     |
                           `----' sequence control                                                    |
                           `----- note on/off ------ (o)                                              |
                           `----- I program change ----- (i)                                          |
+----------------+                                                                                    |
| <<container>>  |                                                                                    |
|  snd_PitchBend | ---> (A streamer) -----------> arpeggio/pitch-bend stream ---> (A decoder) --------'
+----------------+         ↑    |                                                                     |
                           `----' sequence control                                                    |
                           `----- note on/off ------ (o)                                              |
                           `----- A program change ----- (a)                                          |
+----------------+                                                                                    |
| <<container>>  |                                                                                    |
| snd_PeriodBend | ---> (P streamer) -----------> period modification stream ---> (P decoder) --------'
+----------------+         ↑    |
                           `----' sequence control
                           `----- note on/off ------ (o)
                           `----- P program change ----- (p)
```


## Sound data structure

The below table shows typical data types of SNDDRV and corresponding XML data
type of Arkos Tracker 2 song file (.aks).

| SNDDRV           | Arkos Tracker 2 (.aks) | explanation                                        |
|------------------|------------------------|----------------------------------------------------|
| snd\_SoundAssets | aks:songType           | Collection of sound assets                         |
| snd\_PitchBend   | aks:arpeggioType       | Arpegio / Pitch-bend table                         |
| snd\_PeriodBend  | aks:pitchType          | Period / Wavelength modification table             |
| snd\_Instrument  | aks:fmInstrumentType   | Musical instrument / Timbre                        |
| snd\_Music       | aks:subsongType        | Music                                              |
| snd\_SpeedTrack  | aks:speedTrackType     | Special track for wait tick counts.                |
| snd\_EventTrack  | aks:eventTrackType     | Special track for event signals.                   |
| snd\_Track       | aks:trackType          | A series of musical notes and effects.             |
| snd\_Pattern     | aks:patternType        | Determines which track is played on which channel. |

~~~ mermaid
classDiagram
snd_SoundAssets *-- "0..*" snd_PitchBend  : list of arpeggio tables
snd_SoundAssets *-- "0..*" snd_PeriodBend : list of period tables
snd_SoundAssets *-- "1..*" snd_Instrument : list of timbres
snd_SoundAssets *-- "1..*" snd_Music      : list of musics

snd_Music *-- "0..*" snd_SpeedTrack : speedTracks
snd_Music *-- "0..*" snd_EventTrack : eventTracks
snd_Music *-- "1..*" snd_Track      : tracks
snd_Music *-- "1..*" snd_Pattern    : pattern sequence

snd_SpeedTrack "1" <.. snd_Pattern : refers by index
snd_EventTrack "1" <.. snd_Pattern : refers by index
snd_Track      "3" <.. snd_Pattern : refers by index

snd_PitchBend  <.. snd_Track : refers by index
snd_PeriodBend <.. snd_Track : refers by index
snd_Instrument <.. snd_Track : refers by index
~~~

## Sound data format notation

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

## Definition of Sound data

### Definition of SpeedTrack stream

(snip)

### Definition of EventTrack stream

(snip)

### Definition of Track stream

The following is the sound data format for **track streams** written in the
notation defined in the previous section.

#### Track stream
```
; Track stream
t_stream   := t_line*

; A line of track
t_line     := t_chunk* EOL

; end mark of line, and number of lines to be skipped.
EOL        := 110b l:5            ; l: number of lines to be skipped (0..31)
```

#### Chunk of Track stream
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
            | 1100b x:4 y:4 _:4   ; force the speed of an arpeggio / pitch-bend table (1 step for each xy+1 ticks)
            | 1101b x:4 y:4 _:4   ; force the speed of a period / wavelength table (1 step for each xy+1 ticks)
            | 1110b x:4 y:4 _:4   ; set arpeggio / pitch-bend table # to xy (1..255) or turn off (xy = 0)
            | 1111b x:4 y:4 _:4   ; set period / wavelength table # to xy (1..255) or turn off (xy = 0)
```

### Definition of Pattern table

(snip)

## Definition of Instrument (timbre) table

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
R  := r:3 T:1 0111b                           ; (rrrT0111) r = ratio (0..7), T = retrigger?
  ; Ratio
  ;    hw_period = sw_period >> r   if modulation type is "soft to hard"
  ;    sw_period = hw_period << r   if modulation type is "hard to soft"
  ;    r is unused                  otherwise
  ; Retrigger?
  T := 0 ; do not restart hardware envelope if its waveform is same as before.
    |  1 ; force to restart hardware envelope.

Ps := _00b 01111b period:16
   |  _01b 01111b period_delta:16
   |  _10b 01111b pitch_delta:16
   |  _11b 01111b period_delta:16 pitch_delta:16

Ph := 00b 011111b period:16
   |  01b 011111b period_delta:16
   |  10b 011111b pitch_delta:16
   |  11b 011111b period_delta:16 pitch_delta:16
```
