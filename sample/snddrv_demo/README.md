# sample: SNDDRV demo


<https://user-images.githubusercontent.com/659805/200720906-7409fd6c-ee62-4f37-89bf-ba6645ebfc65.mp4>


This sample shows usecase of SNDDRV a PSG sound driver.

SNDDRV covers almost feature of replayer of the [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).

SNDDRV and libmsx's build system make it easy to integrate Arkos Tracker 2 song
file (.aks file) into your application.

Have you Arkos Tracker 2 song file (.aks file) ?

Do you want to try with your .aks file ?

If so, try the following:

1. Remove `src/bgm.aks`
2. Rename your .aks file to `bgm.aks` and place it as `src/bgm.aks`
3. Do `make clean all`, and `bin/snddrv_demo.rom` is made.
4. Then run `bin/snddrv_demo.rom` on your favorite MSX emulator.

At step 3, also `src/bgm.h` and `src/bgm.c` are automatically generated from
`src/bgm.aks` as intermediate C source files. See also them If you have
interested.

See also
- [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).
- libmsx API reference
  - [snddrv.h](https://mori0091.github.io/libmsx/snddrv_8h.html)
  - [snd_sound.h](https://mori0091.github.io/libmsx/snd__sound_8h.html)
- "Build instructions of user applications" section of libmsx's [README.md](https://github.com/mori0091/libmsx)
- [DESIGN NOTE: SNDDRV - a sound driver](https://github.com/mori0091/libmsx/blob/main/docs/design-notes_snddrv.md)
- [DESIGN NOTE: Internal of SNDDRV](https://github.com/mori0091/libmsx/blob/main/docs/design-notes_snddrv_internal.md)
