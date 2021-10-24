# libmsx ; C library for MSX

The `libmsx` library exposes MSX, MSX2, MSX2+, and MSXturboR hardware
functionality to the C programmer.

The library is intended for use with the C cross compiler
[SDCC](http://sdcc.sourceforge.net/), and makes it easy to build 16KiB /
32KiB ROM cartridge image.

> MSX is a registered trademark of MSX Licensing Corporation.

## Supported functionality

The below functionality is supported.

  - Getting MSX system version code

  - VSYNC based timing synchronization

  - Almost of TMS9918A, V9938, V9958 VDP (Video Display Processor)
    functionality.
    
      - Display screen mode setting
      - Display area position adjustment
      - Display lines setting (192 lines / 212 lines)
      - VRAM memory map setting (pattern name table, color table, and so
        on)
      - Color palette
      - Hardware vertical scroll
      - Hardware horizontal scroll
      - Enable / disable display visibility
      - Enable / disable sprites visibility
      - VDP command execution (HMMM, HMMV, LMMM, and LMMV)
      - VRAM read / write
      - … and so on

  - GPIO (8-bit parallel I/O port ; a.k.a Joystick I/F)
    
      - Reading joystick input state.

## Unsupported functionality

The below functionality is NOT supported yet.

  - Keyboard input
  - Sound output
      - PSG (AY-3-8910)
      - OPLL (YM2413 / MSX-MUSIC)
  - BGM driver
  - Sound effect driver
  - … and so on

# Pre-requisites

## Target Machine

  - MSX
  - MSX2
  - MSX2+
  - MSXturboR

## Host Platform

  - Ubuntu 20.04
  - Ubuntu 21.xx (not tested)

Ofcourse, Ubuntu on **WSL** is okey. :smile:  
(**WSL**: Windows Subsystem for Linux)

## Build tools

  - SDCC  
    Small Device C Compiler  
    <http://sdcc.sourceforge.net/>

… and some popular standard Linux/Unix commands:

  - `find`, `make`,
  - and so on.

## Install instructions of Build tools

**Ubuntu**

``` shell
sudo apt update
sudo apt install build-essential
sudo apt install sdcc
```

# Build instructions of the library and sample projects

The below makes the library `libmsx.lib` into `lib` directory.

``` shell
make
```

To build sample projects in `sample/`, do as follows:

``` shell
make sample
```

And for cleaning up:

``` shell
make clean
```
