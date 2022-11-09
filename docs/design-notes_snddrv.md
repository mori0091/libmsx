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
  - A support tool `aks2c.sh` translates Arkos Tracker 2 song file (.aks) to C
    source file.
  - The `libmsx`'s build system (makefiles) can automatically translates .aks
    files into C source files and built them into your application. It automates
    the following workflow.
    1. Translate .aks files into C source files.
    2. Compile translated C source files, all other C and ASM source files.
    3. Build all of them into your application.

# How to integrate Arkos Tracker 2 .aks files into your application

1. Make music with Arkos Tracker 2, and save as .aks file.
2. Place .aks file(s) into `src` folder of your repository.
3. Write application source code, for example such as the following, and place
   it into `src` folder:

> Note that the filename of .aks file is also used as name of corresponding C
> source files and/or name of variable of `snd_SoundAssets` type. Therefore it
> must be a legal identifier in C language.
>
> For example, `my_bgm.aks`, `playlist000.aks` are okay.
> 
> But `my bgm.aks`, `2022_Greatest_Hits.aks`, `My favorite songs.aks` are invalid.

~~~c
/*
 * \file main.c
 * 
 * \note
 *   This code is assuming that the .aks file is named as "my_bgm.aks".
 */

#include <msx.h>

// "my_bgm.h" is automatically generated from "my_bgm.aks".
#include "my_bgm.h"

void main(void) {
  // Initialize the sound driver.
  // This must be called once at least, before the snd_play() is called.
  snd_init();

  // Register the sound driver as VSYNC handler.
  set_vsync_handler(snd_play);

  // Register the music #0 in the my_bgm as background music.
  // \note
  //   A .aks file and a snd_SoundAssets object can contain multiple music.
  //   Each music is numbered #0, #1, #2, ...
  snd_set_bgm(0, &my_bgm);

  // Start (or resume if paused) to play music.
  snd_start();
  
  // main-loop
  for (;;) {
    // ...
    // Play the music #1 in the my_bgm as sound effects.
    // snd_set_sfx(1, &my_bgm);
  }
}
~~~

See also
- [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).
- [snddrv_demo](https://github.com/mori0091/libmsx/tree/main/sample/snddrv_demo) the sample project.
- libmsx API reference
  - [snddrv.h](https://mori0091.github.io/libmsx/snddrv_8h.html)
  - [snd_sound.h](https://mori0091.github.io/libmsx/snd__sound_8h.html)
- "Build instructions of user applications" section of libmsx's [README.md](https://github.com/mori0091/libmsx)
- [DESIGN NOTE: Internal of SNDDRV](design-notes_snddrv_internal.md)
