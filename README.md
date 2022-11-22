# libmsx ; C library for MSX

[![MIT License](https://img.shields.io/github/license/mori0091/libmsx.svg)](https://github.com/mori0091/libmsx/blob/main/LICENSE)
[![C/C++ CI](https://github.com/mori0091/libmsx/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mori0091/libmsx/actions/workflows/c-cpp.yml)
[![Codacy Security Scan](https://github.com/mori0091/libmsx/actions/workflows/codacy-analysis.yml/badge.svg)](https://github.com/mori0091/libmsx/actions/workflows/codacy-analysis.yml)

The `libmsx` library exposes MSX, MSX2, MSX2+, and MSXturboR hardware
functionality to the C programmer.

The library is intended for use with the C cross compiler
[SDCC](http://sdcc.sourceforge.net/), and makes it easy to build MSX ROM
cartridge image (16KiB, 32KiB, or MegaROM).

> MSX is a registered trademark of MSX Licensing Corporation.

_Pull requests (PRs) and issues (bug reports, feature requests) are welcome._ 😉

## LICENSE

Copyright (c) 2021-2022 Daishi Mori (mori0091)

This software is released under the MIT License.  
See <https://github.com/mori0091/libmsx/blob/main/LICENSE>

GitHub libmsx project  
<https://github.com/mori0091/libmsx>

> A code snippet (gsinit.s) extracted and derived from the SDCC runtime library
> are available under the GPLv2 license.  
> See <https://github.com/mori0091/libmsx/blob/main/sdcc/README.md>.

## API reference

The API reference of the `libmsx` library is hosted on GitHub Pages.  
See <https://mori0091.github.io/libmsx/index.html>

## Supported functionality

The below functionality is supported.

- Getting MSX system version code

- VSYNC based timing synchronization

- Almost of TMS9918A, V9938, V9958 VDP (Video Display Processor) functionality.
  - Display screen mode setting
  - Display area position adjustment
  - Display lines setting (192 lines / 212 lines)
  - VRAM memory map setting (pattern name table, color table, and so on)
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

- Sound output
  - PSG (AY-3-8910)
  - **NEW** Konami SCC/SCC+ Sound Cartridge
    - SCC/SCC+ sound cartridge inspection, discovery,
    - Enabling/Disabling register access, and
    - Read/Write registers.

- Sound driver
  - SNDDRV a PSG sound driver
    - SNDDRV covers almost feature of replayer of the [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).
    - SNDDRV and libmsx's build system make it easy to integrate Arkos Tracker 2 song
file (.aks file) into your application.
    - See also the [design note of SNDDRV](docs/design-notes_snddrv.md)

  - Another OLD PSG sound driver (sound.h, sound_eg.h) is still available,
    but is no longer maintained.

- MegaROM Mapper
  - SDCC `__banked` functions support (SDCC 4.2.0 or later)
  - Supported ROM mappers:
    - ASCII8 ROM mapper
    - ASCII16 ROM mapper

- **NEW** Utility functions for inspecting MSX slot mechanisms.
  - Various inspection functions; inspect slot contents.
    - MAIN ROM, SUB-ROM, BDOS/FDC,
    - MSX-MUSIC, SCC/SCC+,
    - Internal RAM,
    - ROM cartridges.
  - Iteration function; invoke user-defined callbacks for each slot.
 
## Unsupported functionality

The below functionality is NOT supported yet.

- Keyboard input
- Sound output
  - ~~PSG (AY-3-8910)~~ `supported`
  - OPLL (YM2413 / MSX-MUSIC)
- ~~BGM driver~~ `supported`
- ~~Sound effect driver~~ `supported`
- … and so on

# Pre-requisites

## Target Machine

- MSX
- MSX2
- MSX2+
- MSXturboR

## Host Platform

**Linux**

| Host Platform | SDCC version | status          |
|---------------|--------------|-----------------|
| Ubuntu 22.04  | 4.0.0        | ✔ (outdated)    |
|               | 4.1.0        | (not tested)    |
|               | 4.2.0        | ✔ (recommended) |

**Windows**

| Host Platform | SDCC version | status          |
|---------------|--------------|-----------------|
| MSYS2 UCRT64  | 4.0.0        | (not tested)    |
|               | 4.1.0        | (not tested)    |
|               | 4.2.0        | ✔ (recommended) |

Ofcourse, Ubuntu on **WSL** is okey. 😄  
(**WSL**: Windows Subsystem for Linux)

## Build tools

- SDCC  
  Small Device C Compiler  
  <http://sdcc.sourceforge.net/>

> SDCC 4.2.0 or later is recommended.

- Other toolchain required for libmsx  
  GNU `find`, `make`, `gcc`, `gzip`, `zcat`, `xsltproc`, and so on.


# Install instructions of Build tools

To use `libmsx`,
1. Install SDCC.
2. Install other toolchain.

## Ubuntu 22.04

### 1. Install SDCC

Download a SDCC pre-built binary for Linux from <http://sdcc.sourceforge.net/>
and install it.

SDCC 4.2.0 or later is recommended.

Don't forget adding SDCC to `PATH` environment variable.  
Add the below to `~/.bashrc` or `~/.bash_profile`.  

> At here it is assuming that the SDCC 4.2.0 is installed under `~/sdcc-4.2.0`.

```shell
export PATH=${PATH}:~/sdcc-4.2.0/bin
```

> Why don't we install sdcc by `sudo apt install sdcc` ?
> 
> Because the latest version of SDCC may not be installed by `apt`.
> In case of Ubuntu 22.04, SDCC 4.0.0 is installed. Thus we
> recommend that you download SDCC 4.2.0 pre-built binary and use it on Ubuntu
> 22.04 or later.

### 2. Install other toolchain

``` shell
sudo apt update
sudo apt install build-essential make gcc gzip xsltproc git
```

## Windows (Ubuntu on WSL2)

The 1st case is using Ubuntu on WSL2 environment.  
In this case, it is same as for Ubuntu 22.04.

Launch a terminal of *Ubuntu on WSL2*, and proceed with the above instructions.


## Windows (MSYS2)

The 2nd case is using MSYS2 UCRT64 environment.

### 1. Install SDCC

Download a SDCC pre-built binary for Windows from <http://sdcc.sourceforge.net/>
and install it.

### 2. Install other toolchain

Download and install [MSYS2](https://www.msys2.org/).

Then launch a terminal for the MSYS2 UCRT64 environment, and do as follows:
~~~ shell
pacman -S coreutils findutils make gcc gzip libxslt git
~~~

Don't forget adding SDCC to `PATH` environment variable.  
Add the below to `~/.bashrc` or `~/.bash_profile`, on MSYS2 UCRT64 environment.  

> At here it is assuming that the SDCC 4.2.0 is installed under `C:\Program Files\SDCC`.

```shell
export PATH=${PATH}:/c/Program\ Files/SDCC/bin
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

# Build instructions of user applications

> There are several sample projects, please refer to the `sample/`
> folder of the `libmsx` project.

## Preparation

**Step 1.** Make your project's Git repository.

``` shell
git init my_proj
```

**Step 2.** Download `libmsx` and place it somewhere.

``` shell
git clone https://github.com/mori0091/libmsx.git libmsx
```

**Step 3.** Copy `libmsx/mk/Makefile` to top of your repository, and
customize it.

See <https://github.com/mori0091/libmsx/blob/main/mk/Makefile>

``` shell
cp libmsx/mk/Makfile my_proj/
```

**Step 4.** Create the folder `my_proj/src/` and place your C source
files under it or in its subfolders.

``` shell
mkdir my_proj/src
```

## Build your application

To build your application, do as follows.  
Then the ROM image will be made into `my_proj/bin` folder.

``` shell
make
```

And for cleaning up:

``` shell
make clean
```
