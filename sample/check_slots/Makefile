# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# ----------------------------------------------------------------------------
# This "Makefile" was made based on a template distributed by libmsx project.
#
# The original template is in `mk/` folder of libmsx GitHub repository.
# libmsx on GitHub: https://github.com/mori0091/libmsx
# ----------------------------------------------------------------------------

# [REQUIRED]
# Path to top of the libmsx installed folder.
LIBMSX_HOME = ../..

# [REQUIRED]
# Specify NAME of your rom image to be built.
#   The rom image `${NAME}.rom` will be made into ${BINDIR} folder.
#   e.g. If NAME was foo, the image will be `bin/foo.rom`.
NAME = check_slots

# [OPTIONAL]
# You may specify additional C compiler/linker options.
#   - CFLAGS shall be C language options such as "-I dir"
#   - LDFLAGS shall be linker options such as "-L dir"
#   - LDLIBS shall be libraries to be linked such as "-lfoo" or "foo.lib"
#   - LIBS shall be your custom libraries to be linked such as "mylib.lib"
CFLAGS  = -DNDEBUG --std-c11
LDFLAGS =
LDLIBS  =
LIBS    =

# [OPTIONAL]
# You may specify a folder where your C source code exists.
# (default is 'src', if omitted)
# SRCDIR = src

# [OPTIONAL]
# You may specify a folder where object code would to be made.
# (default is 'obj', if omitted)
# OBJDIR = obj

# [OPTIONAL]
# You may specify a folder where your rom image would to be made.
# (default is 'bin', if omitted)
# BINDIR = bin

# [OPTIONAL]
# If `USE_ALL_EXTENSIONS` is set to `1`, all connected system extensions (e.g.
# disk drive, RS-232C cartridge) are initialized and ready for use before your
# `main()` function starts. Maybe some RAM space and startup time are consumed
# though.
# (default is undefined, if omitted)
USE_ALL_EXTENSIONS = 1

# [REQUIRED]
# Include one of the configuration.
#   - ${LIBMSX_HOME}/mk/16k.4000.mk : to make 16KB rom image (0x4000-0x7fff)
#   - ${LIBMSX_HOME}/mk/32k.4000.mk : to make 32KB rom image (0x4000-0xbfff)
#   - ${LIBMSX_HOME}/mk/ascii16.mk  : to make MegaROM image (ROM type: ASCII16)
#   - ${LIBMSX_HOME}/mk/ascii8.mk   : to make MegaROM image (ROM type: ASCII8)
include ${LIBMSX_HOME}/mk/16k.4000.mk

# [REQUIRED] (No need to modify)
# Include common part of Makefile
include ${LIBMSX_HOME}/mk/build.mk
