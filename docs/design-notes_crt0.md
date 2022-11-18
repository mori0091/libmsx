<!-- -*- coding: utf-8-unix -*- -->
# DESIGN NOTE : crt0 - C startup routine

The `crt0` - C startup routine - shall be configured accoring to ROM image size
and/or ROM type.

To configure ROM image size and/or ROM type for your application, define
`CONFIG_ROM_TYPE` variable in your `Makefile` as follows:

~~~ makefile
# (in your project's Makefile)
# Select ROM type.
# - 16k     : to make 16KiB rom image (0x4000-0x7fff)
# - 32k     : to make 32KiB rom image (0x4000-0xbfff)
# - ascii16 : to make MegaROM image (ROM type: ASCII16)
# - ascii8  : to make MegaROM image (ROM type: ASCII8)
CONFIG_ROM_TYPE = 16k
~~~

If you want to use BDOS (MSX-DOS / Disk BASIC) functionality in your
application, in addition to the above, set `CONFIG_CRT0_MOD_INIT` to `init_x` or
`init_r`.
~~~ makefile
# (in your project's Makefile)
CONFIG_CRT0_MOD_INIT = init_x
~~~

# `make` variables for crt0 configuration

## CONFIG\_ROM\_TYPE

Select ROM type of the ROM image to be built.

`CONFIG_ROM_TYPE ?= 16k` in default.

| value     | explanation                                                        |
|-----------|--------------------------------------------------------------------|
| `16k`     | 16KiB plain ROM image, starting at 0x4000.                         |
| `32k`     | 32KiB plain ROM image, starting at 0x4000.                         |
| `ascii8`  | MegaROM image with ASCII8 rom mapper support, starting at 0x4000.  |
| `ascii16` | MegaROM image with ASCII16 rom mapper support, starting at 0x4000. |

## CONFIG\_CRT0\_MOD\_ROM\_HEADER

| value                | explanation                                    |
|----------------------|------------------------------------------------|
| `app_rom_header`     | ROM header for plain ROM image of application. |
| `app_megarom_header` | ROM header for MegaROM image of application.   |

The value is automatically selected according to `CONFIG_ROM_TYPE`.

## CONFIG\_CRT0\_MOD\_INIT

Select `init` module of the ROM image to be built.

`CONFIG_CRT0_MOD_INIT ?= init_0` in default.

`init_0` starts `main()` immediately on initialization phase of the ROM
cartridge. No other system extension is initialized.

 In contrast to `init_0`, `init_x` or `init_r` starts `main()` after
 initialization phase of all system extension is done. This means that:
- all system extension is ready to use. if it exists,
- but maybe, some RAM space and startup time are consumed.

The table below shows the runtime environment at application startup:

| value    | CPU mode            | BDOS                                 | other system extension |
|----------|---------------------|--------------------------------------|------------------------|
| `init_0` | Z80 mode            | N/A                                  | N/A                    |
| `init_x` | Z80 mode            | MSX-DOS 1 / Disk BASIC 1.x [^1]      | available if exists    |
| `init_r` | R800 DRAM mode [^2] | MSX-DOS 2 / Disk BASIC 2.x [^1] [^2] | available if exists    |

[^1]: Available only if a FDC exists.  
      If `Shift` key was pressed during startup time, BDOS is not available even
      if a Floppy Disk Controller (FDC) exists.

[^2]: `MSXturboR` or later.  
      On `MSX`, `MSX2`, or `MSX2+` the behaviour is same as `init_x`.  
      If `1` key was pressed during startup time, the behaviour will be same as
      `init_x` even if on `MSXturboR`.

## CONFIG\_CRT0\_MOD\_START

| value           | explanation                                |
|-----------------|--------------------------------------------|
| `start_16k`     | startup routine for plain 16KiB ROM image. |
| `start_32k`     | startup routine for plain 32KiB ROM image. |
| `start_megarom` | startup routine for MegaROM image.         |

The value is automatically selected according to `CONFIG_ROM_TYPE`.

## CONFIG\_CRT0\_MOD\_ROM\_MAPPER

| value         | explanation            |
|---------------|------------------------|
| `rom_ascii8`  | use ASCII8 ROM mapper  |
| `rom_ascii16` | use ASCII16 ROM mapper |

The value is automatically selected according to `CONFIG_ROM_TYPE`.

## CONFIG\_CRT0\_MOD\_LIBS

| value            | explanation                      |
|------------------|----------------------------------|
| `get_slot_page1` | get the current slot of page #1. |
| `set_slot_page2` | switch the slot of page #2.      |

The value is automatically selected according to `CONFIG_ROM_TYPE` and
`CONFIG_CRT0_MOD_INIT`.

## USE\_ALL\_EXTENSIONS (deprecated)

`USE_ALL_EXTENSIONS ?=` in default.

`USE_ALL_EXTENSIONS = 1` implies `CONFIG_CRT0_MOD_INIT = init_x` for backward
compatibility.


# Pre-defined crt0 configurations

Some pre-defined crt0 configurations are in `${LIBMSX_HOME}/mk/` folder.

Instead of defining `CONFIG_ROM_TYPE`, you can `include` one of the following
into the `Makefile`.
- `${LIBMSX_HOME}/mk/16k.4000.mk`
- `${LIBMSX_HOME}/mk/32k.4000.mk`
- `${LIBMSX_HOME}/mk/ascii8.mk`
- `${LIBMSX_HOME}/mk/ascii16.mk`

The below table shows that which component is integrated into `crt0`, for each
pre-defined configuration:

|                              | 16k.4000.mk           | 32k.4000.mk      | ascii8.mk            | ascii16.mk           |
|------------------------------|-----------------------|------------------|----------------------|----------------------|
| `CONFIG_CRT0_MOD_ROM_HEADER` | `app_rom_header`      | `app_rom_header` | `app_megarom_header` | `app_megarom_header` |
| `CONFIG_CRT0_MOD_INIT`       | [^3]                  | [^3]             | [^3]                 | [^3]                 |
| `CONFIG_CRT0_MOD_START`      | `start_16k`           | `start_32k`      | `start_megarom`      | `start_megarom`      |
| `CONFIG_CRT0_MOD_ROM_MAPPER` |                       |                  | `rom_ascii8`         | `rom_ascii16`        |
| `CONFIG_CRT0_MOD_LIBS`       | `get_slot_page1` [^4] | `get_slot_page1` | `get_slot_page1`     | `get_slot_page1`     |
|                              |                       | `set_slot_page2` | `set_slot_page2`     | `set_slot_page2`     |

[^3]: Choose one of the `init` module; `init_0`, `init_x`, or `init_r`. `init_0`
is selected in default.

[^4]: Selected (implied) only if `init_x` or `init_r` is selected as the init
module.

## \${LIBMSX\_HOME}/mk/16k.4000.mk

~~~ makefile
CONFIG_ROM_TYPE = 16k
CONFIG_CRT0_MOD_INIT ?= init_0
~~~

## \${LIBMSX\_HOME}/mk/32k.4000.mk

~~~ makefile
CONFIG_ROM_TYPE = 32k
CONFIG_CRT0_MOD_INIT ?= init_0
~~~

## \${LIBMSX\_HOME}/mk/ascii8.mk

~~~ makefile
CONFIG_ROM_TYPE = ascii8
CONFIG_CRT0_MOD_INIT ?= init_0
~~~

## \${LIBMSX\_HOME}/mk/ascii16.mk

~~~ makefile
CONFIG_ROM_TYPE = ascii16
CONFIG_CRT0_MOD_INIT ?= init_0
~~~
