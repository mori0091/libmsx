# -*- coding: utf-8-unix; tab-width: 8 -*-

# ---------------------------------------------------------------
# This "Makefile" is a template to make application for MSX.
# Place a copy of this file on your top of project folder.
# ---------------------------------------------------------------

# Specify NAME of your rom image to be built.
#   The rom image `${NAME}.rom` will be made into ${BINDIR} folder.
#   e.g. If NAME was foo, the image will be `bin/foo.rom`.
NAME = hello

# Specify a folder where your C source code exists. (default 'src')
# SRCDIR = src

# Specify a folder where object code would to be made. (default 'obj')
# OBJDIR = obj

# Specify a folder where your rom image would to be made. (default 'bin')
# BINDIR = bin

# You may specify additional C compiler/linker options.
#   - CFLAGS shall be C language options such as "-I dir"
#   - LDFLAGS shall be linker options such as "-L dir"
#   - LDLIBS shall be libraries to be linked such as "-lfoo" or "libfoo.lib"
CFLAGS  = -DNDEBUG
LDFLAGS =
LDLIBS  =

# Select startup routine.
#   - libmsx/lib/16k.4000.rel : for making MSX 16KB rom image (0x4000-0x7fff)
#   - libmsx/lib/32k.4000.rel : for making MSX 32KB rom image (0x4000-0x7fff)
CRT0 = libmsx/lib/32k.4000/crt0.rel

# Include one of the configuration.
#   - libmsx/mk/16k.4000.mk : for making MSX 16KB rom image (0x4000-0x7fff)
#   - libmsx/mk/32k.4000.mk : for making MSX 32KB rom image (0x4000-0xbfff)
include libmsx/mk/32k.4000.mk

# Include common part of Makefile
include libmsx/mk/build.mk
