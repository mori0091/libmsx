# sample: Horizontal Scroll (MSX2+) / MegaROM version

This sample shows usecase / implementation example of the following feature:

- Fixed score board with scrolling screen
  - V9958 Hardware horizontal scroll (MSX2+ or later)
  - HSYNC interrupt
  - VSYNC interrupt

- Color palette animation
  - color palette
  - VSYNC interrupt

- Background music and Sound effects
  - PSG sound driver
  - VSYNC interrupt

- MegaROM / SDCC __banked functions (SDCC 4.2.0 or later)
  - application is devided into 3 parts
    - CODE (0x4010..0x7fff ; fixed)
      - `void main(void)` (entry point of the application)
      - sound data
      - user-defined interrupt handler
      - global `const` variables
      - library functions (always placed in CODE section by linker)
        - `libmsx` library functions 
        - C standard library functions
        - SDCC runtime library functions
    - BANK0 (0x8000..0xbfff ; banked / switchable by `__banked` function call)
      - Describe `pragma codeseg BANK0` in all `*.c`
      - `void game_main(void) __banked` (entry point of main module)
      - BANK0 local functions.
      - BANK0 local const variables.
    - BANK1 (0x8000..0xbfff ; banked / switchable by `__banked` function call)
      - Describe `pragma codeseg BANK1` in all `*.c`
      - `void boot_main(void) __banked` (entry point of boot module)
      - BANK1 local functions for VRAM initialization.
      - BANK1 local const variables.

~~~
sample/megarom/
|-- Makefile
|-- README.md           ... this file you read
|-- include/            ... header files
|   |-- anime.h
|   |-- boot_main.h
|   |-- game_main.h
|   |-- interrupt_handler.h
|   |-- macros.h
|   `-- sound_data.h
`-- src/
    |-- bank0/          ... C source files for BANK0 section
    |   `-- game_main.c
    |-- bank1/          ... C source files for BANK1 section
    |   `-- boot_main.c
    `-- code/           ... C source files for CODE section
        |-- anime.c
        |-- interrupt_handler.c
        |-- main.c
        `-- sound_data/
            |-- bgm_1.c
            `-- se_1.c
~~~

> _NOTE_  
> This sample is very small, plain 16KiB ROM is enough.  
> If you want do so, try the following:
> - remove all `__banked` keyword and `pragma codeseg BANK*` in the `*.c` and `*.h`
> - modify `Makefile`
>   - from `#include ${LIBMSX_HOME}/mk/ascii16.mk`
>   -   to `#include ${LIBMSX_HOME}/mk/16k.4000.mk`
