# DESIGN NOTE : LA0 - The libmsx audio format 0.

_LA0_ is a file/data format and its decoder developed for use with "the libmsx
audio replayer".

This article describes the _LA0_ file/data format in detail.

First, the "data format notation" used in this article will be explained,
followed by a description of the _LA0_ file/data format using that notation.


# Data format notation

Here is the notation for defining our data format.  
This notation itself is described in BNF notation.

~~~ BNF
<definition>
       ::= <identifier> "=" <term>

<term> ::= <identifier>        ; name of a variable
         | <term> <term>       ; sequence of <term>
         | "(" <term> ")"      ; group of <term>
         | <term> "|" <term>   ; left-hand <term> or right-hand <term>
         | <term> "?"          ; 0 or 1 <term>
         | <term> "*"          ; 0 or more <term>
         | <term> "+"          ; 1 or more <term>
         | "'" "a" "'"         ; 8 bit ASCII character 'a'
         | <digits>            ; number
         | <term> ":" <T>      ; <term> of type <T>

<comment>                      ; The characters "//" and up to the end of the
       ::= "//" <any>* <eol>   ; line are comments.
~~~

The following table shows the type notation.

> This is similar to type annotation in the Rust programming language.

| type        | explanation                    |
|-------------|--------------------------------|
| i&#8288;_N_ | _N_ bits signed integer        |
| u&#8288;_N_ | _N_ bits unsigned integer      |
| [_T_;_N_]   | _N_ elements array of type _T_ |

Note that the byte order of multi-byte integer types such as `u16`, `u32`, etc.
is little-endian.


# LA0 file/data format

~~~ BNF
LA0_FILE
    = LA0_SONG*

LA0_SONG
    = 'L' 'A' '0' ' '      // 4-byte tag.
      size:u32             // Size of LA0_SONG_BODY.
      LA0_SONG_BODY

LA0_SONG_BODY
    = STREAM_INFO          // Properties/Attributes of the song.
      SCC_WAVE_TABLE?      // (optional SCC/SCC+ waveform table)
      STREAM_DATA
~~~

## STREAM\_INFO

~~~ BNF
STREAM_INFO
    = rate:u8              // Sampling rate in Hz.
      chip_enable:u8       // Using sound chips.
      total_samples:u16    // Number of samples in total.
      loop_samples:u16     // Number of samples of loop part.
      channel_mask:u8      // Using channels of PSG and SCC/SCC+.
      0:u8                 // (reserved)

chip_enable
    = 0:u4                 // (reserved)
      opll_enable:u1       // 0: disabled, 1: enabled ; OPLL.
      psg_enable:u1        // 0: disabled, 1: enabled ; PSG.
      scci_enable:u1       // 0: disabled, 1: enabled ; SCC+.
      scc_enable:u1        // 0: disabled, 1: enabled ; SCC/SCC+.

channel_mask
    = scc_ch_5:u1          // 0: disabled, 1: enabled ; SCC/SCC+ channel #5.
      scc_ch_4:u1          // 0: disabled, 1: enabled ; SCC/SCC+ channel #4.
      scc_ch_3:u1          // 0: disabled, 1: enabled ; SCC/SCC+ channel #3.
      scc_ch_2:u1          // 0: disabled, 1: enabled ; SCC/SCC+ channel #2.
      scc_ch_1:u1          // 0: disabled, 1: enabled ; SCC/SCC+ channel #1.
      psg_ch_3:u1          // 0: disabled, 1: enabled ; PSG channel #3.
      psg_ch_2:u1          // 0: disabled, 1: enabled ; PSG channel #2.
      psg_ch_1:u1          // 0: disabled, 1: enabled ; PSG channel #1.
~~~

## SCC\_WAVE\_TABLE ; SCC/SCC+ waveform table

~~~ BNF
SCC_WAVE_TABLE
    = 'S' 'C' 'C' 'W'      // 4-byte tag.
      size:u32             // Size of waveform table.
      waveform:[[i8;32];n] // Waveform table. size = 32 * n. (n <= 256)
~~~

The optional `SCC_WAVE_TABLE` contains one or more 32-byte waveforms (up to 256
entries).

These are refered by index, as operand of the command `CMD = 0xfa`..`0xfe`.

See also _DICTIONARY_ section for detail of that command.

## STREAM\_DATA

~~~ BNF
STREAM_DATA
    = 'S' 'T' 'R' 'M'      // 4-byte tag.
      size:u32             // Size of SEQUENCE + DICTIONARY
      SEQUENCE             // Sequence of sample length and offset.
      DICTIONARY           // Dictionary of dedicated samples.
~~~

### SEQUENCE

~~~ BNF
SEQUENCE
    = size:u32             // Size of SEQUENCE_BODY
      SEQUENCE_BODY

SEQUENCE_BODY
    = ((EG_LENGTH EG_OFFSET) | EG_ZERO)+
~~~

The decoder shall track the position of samples in the `DICTIONARY`. The
position is initialized to the head of the `DICTIONARY` when a song begins.

A pair of `length` and `offset`; encoded into the `SEQUENCE_BODY`; represents
length of the next sample and offset from the end of the latest sample to the
head of the next sample in the `DICTIONARY`. The decoder tracks the current
position by adding the offset for each sample.

Each `length` and `offset` is in 2 byte unit. So if the decoded `length` was $m$
and `offset` was $n$, the actual value in bytes are $2m$ and $2n$.

The `length` and `offset` are encoded as Elias-Gamma code.

However, Elias-Gamma code encodes a **positive** integer into a bit-string.

So the non-negative integer length $2m$ in bytes and the signed integer offset
$2n$ in bytes are encoded to the `EG_LENGTH` and `EG_OFFSET` as follows:

$$
\begin{align}
  EG_{LENGTH} &= EG(m+1) & (m \geqq 0)
\end{align}
$$

$$
\begin{align}
  EG_{OFFSET} &=
    \begin{cases}
      EG(|2n|+1) & if \ n \geqq 0 \\
      EG(|2n|)   & if \ n \lt 0
    \end{cases} \\
\end{align}
$$

$$
\begin{align}
  EG_{ZERO} &= EG(1) \\
\end{align}
$$

where
- $EG_{LENGTH}$ is `EG_LENGTH`,
- $EG_{OFFSET}$ is `EG_OFFSET`,
- $EG_{ZERO}$ is `EG_ZERO`,
- $EG(x)$ is Elias-Gamma code of positive integer $x$,
- $|x|$ is absolute of $x$.

If the length was `0`, `EG_LENGTH` will be `EG_ZERO` and `EG_OFFSET` will be
omitted.

Finally, if the length of `SEQUENCE_BODY` is not a multiple of 8 bits, some `0`
bits are padded. Therefore, the size of `SEQUENCE_BODY` is expressed in bytes.

### DICTIONARY

~~~ BNF
DICTIONARY
    = SAMPLE*

SAMPLE
    = (CMD:u8 val:u8)+
~~~

`DICTIONARY` is concatnated unique / dedicated `SAMPLE`s.

A `SAMPLE` is series of `CMD` and `val` pair.

The below table shows `CMD`s:

| CMD        | val               | explanation                                                     |
|------------|-------------------|-----------------------------------------------------------------|
| 0x00..0x1f | value of register | Set to SCC/SCC+ ch.1 Waveform registers.                        |
| 0x20..0x3f | ↑                 | Set to SCC/SCC+ ch.2 Waveform registers.                        |
| 0x40..0x5f | ↑                 | Set to SCC/SCC+ ch.3 Waveform registers.                        |
| 0x60..0x7f | ↑                 | Set to SCC/SCC+ ch.4 Waveform registers.                        |
| 0x80..0x9f | ↑                 | Set to SCC+ ch.5 Waveform registers.                            |
| 0xa0..0xa1 | ↑                 | Set to SCC/SCC+ ch.1 FDR\_LO, FDR\_HI register.                 |
| 0xa2..0xa3 | ↑                 | Set to SCC/SCC+ ch.2 FDR\_LO, FDR\_HI register.                 |
| 0xa4..0xa5 | ↑                 | Set to SCC/SCC+ ch.3 FDR\_LO, FDR\_HI register.                 |
| 0xa6..0xa7 | ↑                 | Set to SCC/SCC+ ch.4 FDR\_LO, FDR\_HI register.                 |
| 0xa8..0xa9 | ↑                 | Set to SCC/SCC+ ch.5 FDR\_LO, FDR\_HI register.                 |
| 0xaa       | ↑                 | Set to SCC/SCC+ ch.1 Volume register.                           |
| 0xab       | ↑                 | Set to SCC/SCC+ ch.2 Volume register.                           |
| 0xac       | ↑                 | Set to SCC/SCC+ ch.3 Volume register.                           |
| 0xad       | ↑                 | Set to SCC/SCC+ ch.4 Volume register.                           |
| 0xae       | ↑                 | Set to SCC/SCC+ ch.5 Volume register.                           |
| 0xaf       | ↑                 | Set to SCC/SCC+ Channel mask register.                          |
|------------|-------------------|-----------------------------------------------------------------|
| 0xb0..0xb1 | value of register | Set to PSG ch.1 FDR\_LO, FDR\_HI register.                      |
| 0xb2..0xb3 | ↑                 | Set to PSG ch.2 FDR\_LO, FDR\_HI register.                      |
| 0xb4..0xb5 | ↑                 | Set to PSG ch.3 FDR\_LO, FDR\_HI register.                      |
| 0xb6       | ↑                 | Set to PSG Noise FDR register.                                  |
| 0xb7       | ↑                 | Set to PSG Mixer register.                                      |
| 0xb8       | ↑                 | Set to PSG ch.1 Volume register.                                |
| 0xb9       | ↑                 | Set to PSG ch.2 Volume register.                                |
| 0xba       | ↑                 | Set to PSG ch.3 Volume register.                                |
| 0xbb..0xbc | ↑                 | Set to PSG H/W envelope FDR\_LO, FDR\_HI register.              |
| 0xbd       | ↑                 | Set to PSG H/W envelope number register.                        |
| 0xbe       | (n/a)             | (reserved)                                                      |
| 0xbf       | (n/a)             | (reserved)                                                      |
|------------|-------------------|-----------------------------------------------------------------|
| 0xc0..0xc7 | value of register | Set to OPLL \$00..\$07 register.                                |
| 0xc8..0xcd | (n/a)             | (reserved)                                                      |
| 0xce       | value of register | Set to OPLL \$0e register.                                      |
| 0xcf       | (n/a)             | (reserved)                                                      |
| 0xd0..0xd8 | value of register | Set to OPLL \$10..\$18 register.                                |
| 0xd9..0xdf | (n/a)             | (reserved)                                                      |
| 0xe0..0xe8 | value of register | Set to OPLL \$20..\$28 register.                                |
| 0xe9..0xef | (n/a)             | (reserved)                                                      |
| 0xf0..0xf8 | value of register | Set to OPLL \$30..\$38 register.                                |
| 0xf9       | (n/a)             | (reserved)                                                      |
|------------|-------------------|-----------------------------------------------------------------|
| 0xfa       | index #           | Copy from `SCC_WAVE_TABLE` to SCC/SCC+ ch.1 Waveform registers. |
| 0xfb       | ↑                 | Copy from `SCC_WAVE_TABLE` to SCC/SCC+ ch.2 Waveform registers. |
| 0xfc       | ↑                 | Copy from `SCC_WAVE_TABLE` to SCC/SCC+ ch.3 Waveform registers. |
| 0xfd       | ↑                 | Copy from `SCC_WAVE_TABLE` to SCC/SCC+ ch.4 Waveform registers. |
| 0xfe       | ↑                 | Copy from `SCC_WAVE_TABLE` to SCC+ ch.5 Waveform registers.     |
|------------|-------------------|-----------------------------------------------------------------|
| 0xff       | (n/a)             | (reserved)                                                      |


# References

- [The libmsx audio replayer.](https://mori0091.github.io/libmsx/group__AUDIO.html)
- [LA0 - PSG, SCC/SCC+, OPLL sound driver.](https://mori0091.github.io/libmsx/group__LA0__DECODER.html)
