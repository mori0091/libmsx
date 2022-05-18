# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# ---------------------------------------------------------------
# This "Makefile" is a template to make application for MSX.
# Place a copy of this file under the top folder of your project,
# and customize the copied Makefile for your project.
# ---------------------------------------------------------------

# [REQUIRED]
# Path to top of the libmsx installed folder.
LIBMSX_HOME = ../..

# [REQUIRED]
# Specify NAME of your rom image to be built.
#   The rom image `${NAME}.rom` will be made into ${BINDIR} folder.
#   e.g. If NAME was foo, the image will be `bin/foo.rom`.
NAME = megarom

# [OPTIONAL]
# You may specify additional C compiler/linker options.
#   - CFLAGS shall be C language options such as "-I dir"
#   - LDFLAGS shall be linker options such as "-L dir"
#   - LDLIBS shall be libraries to be linked such as "-lfoo" or "foo.lib"
#   - LIBS shall be your custom libraries to be linked such as "mylib.lib"
CFLAGS  = -DNDEBUG -I ./include
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

# [REQUIRED]
# Include one of the configuration.
#   - ${LIBMSX_HOME}/mk/16k.4000.mk : to make 16KB rom image (0x4000-0x7fff)
#   - ${LIBMSX_HOME}/mk/32k.4000.mk : to make 32KB rom image (0x4000-0xbfff)
#   - ${LIBMSX_HOME}/mk/ascii16.mk  : to make MegaROM image (ROM type: ASCII16)
#   - ${LIBMSX_HOME}/mk/ascii8.mk   : to make MegaROM image (ROM type: ASCII8)
include ${LIBMSX_HOME}/mk/ascii16.mk

# [REQUIRED] (No need to modify)
# Include common part of Makefile
include ${LIBMSX_HOME}/mk/build.mk



# -- for build compile_commands.json --
# case 1: build on WSL, use on Windows (Mingw)
CWD = $(shell wslpath -a -m ${CURDIR})
# case 2: build on Windows (Mingw), use on Windows (Mingw)
# CWD = $(shell cygpath -a -m ${CURDIR})
# default: build and use on same system
# CWD = ${CURDIR}

${OBJDIR}/%.compdb_entry: ${SRCDIR}/%.c
	@echo "    {" > $@
	@echo "        \"command\": \"${CC} -I ${LIBMSX_HOME}/include -I ./include -c $<\"," >> $@
	@echo "        \"directory\": \"${CWD}\"," >> $@
	@echo "        \"file\": \"$<\"" >> $@
	@echo "    }," >> $@

COMPDB_ENTRIES = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.compdb_entry, ${SRCS_C})

compile_commands.json: ${COMPDB_ENTRIES}
	@echo "[" > $@.tmp
	@cat $^ >> $@.tmp
	@sed '$$d' < $@.tmp > $@
	@echo "    }" >> $@
	@echo "]" >> $@
	@rm $@.tmp
