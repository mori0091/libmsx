# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2023 Daishi Mori (mori0091)
#
# This software is released under the MIT License.\n
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project\n
# https://github.com/mori0091/libmsx

.PHONY: all build clean libmsx

all: build

# ------------------------------------------------------------------------------
# crt0 configuration
IMAGE_SIZE ?= 16384
ADDR_HEAD = 0x4000
ADDR_CODE = 0x4010
ADDR_DATA = 0xc000

CONFIG_ROM_TYPE            ?= 16k
CONFIG_CRT0_MOD_ROM_HEADER ?= app_rom_header
CONFIG_CRT0_MOD_INIT       ?= init_0
CONFIG_CRT0_MOD_START      ?= start_16k
CONFIG_CRT0_MOD_ROM_MAPPER ?=
CONFIG_CRT0_MOD_LIBS       ?=

# for backward compatibility
ifeq (${USE_ALL_EXTENSIONS}, 1)
CONFIG_CRT0_MOD_INIT       = init_x
endif

ifeq (${CONFIG_ROM_TYPE}, 16k)
IMAGE_SIZE = 16384
CONFIG_CRT0_MOD_ROM_HEADER = app_rom_header
CONFIG_CRT0_MOD_START      = start_16k
else ifeq (${CONFIG_ROM_TYPE}, 32k)
IMAGE_SIZE = 32768
CONFIG_CRT0_MOD_ROM_HEADER = app_rom_header
CONFIG_CRT0_MOD_START      = start_32k
else ifeq (${CONFIG_ROM_TYPE}, ascii8)
IMAGE_SIZE = 16384
CONFIG_CRT0_MOD_ROM_HEADER = app_megarom_header
CONFIG_CRT0_MOD_START      = start_megarom
CONFIG_CRT0_MOD_ROM_MAPPER = rom_ascii8
else ifeq (${CONFIG_ROM_TYPE}, ascii16)
IMAGE_SIZE = 16384
CONFIG_CRT0_MOD_ROM_HEADER = app_megarom_header
CONFIG_CRT0_MOD_START      = start_megarom
CONFIG_CRT0_MOD_ROM_MAPPER = rom_ascii16
else
IMAGE_SIZE = 16384
CONFIG_CRT0_MOD_ROM_HEADER = app_megarom_header
CONFIG_CRT0_MOD_START      = start_megarom
CONFIG_CRT0_MOD_ROM_MAPPER ?= rom_ascii16
endif

CONFIG_CRT0_MOD_LIBS      += get_slot

ifneq (${CONFIG_CRT0_MOD_ROM_HEADER}, app_megarom_header)
IHX2BIN_FLAGS = -s ${IMAGE_SIZE} -b ${ADDR_HEAD}
else
LDFLAGS += \
	-Wl-b_BANK1=0x18000 \
	-Wl-b_BANK2=0x28000 \
	-Wl-b_BANK3=0x38000 \
	-Wl-b_BANK4=0x48000 \
	-Wl-b_BANK5=0x58000 \
	-Wl-b_BANK6=0x68000 \
	-Wl-b_BANK7=0x78000 \
	-Wl-b_BANK8=0x88000 \
	-Wl-b_BANK9=0x98000 \
	-Wl-b_BANK10=0xa8000 \
	-Wl-b_BANK11=0xb8000 \
	-Wl-b_BANK12=0xc8000 \
	-Wl-b_BANK13=0xd8000 \
	-Wl-b_BANK14=0xe8000 \
	-Wl-b_BANK15=0xf8000 \
	-Wl-b_BANK16=0x108000 \
	-Wl-b_BANK17=0x118000 \
	-Wl-b_BANK18=0x128000 \
	-Wl-b_BANK19=0x138000 \
	-Wl-b_BANK20=0x148000 \
	-Wl-b_BANK21=0x158000 \
	-Wl-b_BANK22=0x168000 \
	-Wl-b_BANK23=0x178000 \
	-Wl-b_BANK24=0x188000 \
	-Wl-b_BANK25=0x198000 \
	-Wl-b_BANK26=0x1a8000 \
	-Wl-b_BANK27=0x1b8000 \
	-Wl-b_BANK28=0x1c8000 \
	-Wl-b_BANK29=0x1d8000 \
	-Wl-b_BANK30=0x1e8000 \
	-Wl-b_BANK31=0x1f8000 \
	-Wl-b_BANK32=0x208000 \
	-Wl-b_BANK33=0x218000 \
	-Wl-b_BANK34=0x228000 \
	-Wl-b_BANK35=0x238000 \
	-Wl-b_BANK36=0x248000 \
	-Wl-b_BANK37=0x258000 \
	-Wl-b_BANK38=0x268000 \
	-Wl-b_BANK39=0x278000 \
	-Wl-b_BANK40=0x288000 \
	-Wl-b_BANK41=0x298000 \
	-Wl-b_BANK42=0x2a8000 \
	-Wl-b_BANK43=0x2b8000 \
	-Wl-b_BANK44=0x2c8000 \
	-Wl-b_BANK45=0x2d8000 \
	-Wl-b_BANK46=0x2e8000 \
	-Wl-b_BANK47=0x2f8000 \
	-Wl-b_BANK48=0x308000 \
	-Wl-b_BANK49=0x318000 \
	-Wl-b_BANK50=0x328000 \
	-Wl-b_BANK51=0x338000 \
	-Wl-b_BANK52=0x348000 \
	-Wl-b_BANK53=0x358000 \
	-Wl-b_BANK54=0x368000 \
	-Wl-b_BANK55=0x378000 \
	-Wl-b_BANK56=0x388000 \
	-Wl-b_BANK57=0x398000 \
	-Wl-b_BANK58=0x3a8000 \
	-Wl-b_BANK59=0x3b8000 \
	-Wl-b_BANK60=0x3c8000 \
	-Wl-b_BANK61=0x3d8000 \
	-Wl-b_BANK62=0x3e8000 \
	-Wl-b_BANK63=0x3f8000

IHX2BIN_FLAGS = -s ${IMAGE_SIZE} -b ${ADDR_HEAD} \
		-s 16384			 \
		-b 0x18000			 \
		-b 0x28000			 \
		-b 0x38000			 \
		-b 0x48000			 \
		-b 0x58000			 \
		-b 0x68000			 \
		-b 0x78000			 \
		-b 0x88000			 \
		-b 0x98000			 \
		-b 0xa8000			 \
		-b 0xb8000			 \
		-b 0xc8000			 \
		-b 0xd8000			 \
		-b 0xe8000			 \
		-b 0xf8000			 \
		-b 0x108000			 \
		-b 0x118000			 \
		-b 0x128000			 \
		-b 0x138000			 \
		-b 0x148000			 \
		-b 0x158000			 \
		-b 0x168000			 \
		-b 0x178000			 \
		-b 0x188000			 \
		-b 0x198000			 \
		-b 0x1a8000			 \
		-b 0x1b8000			 \
		-b 0x1c8000			 \
		-b 0x1d8000			 \
		-b 0x1e8000			 \
		-b 0x1f8000			 \
		-b 0x208000			 \
		-b 0x218000			 \
		-b 0x228000			 \
		-b 0x238000			 \
		-b 0x248000			 \
		-b 0x258000			 \
		-b 0x268000			 \
		-b 0x278000			 \
		-b 0x288000			 \
		-b 0x298000			 \
		-b 0x2a8000			 \
		-b 0x2b8000			 \
		-b 0x2c8000			 \
		-b 0x2d8000			 \
		-b 0x2e8000			 \
		-b 0x2f8000			 \
		-b 0x308000			 \
		-b 0x318000			 \
		-b 0x328000			 \
		-b 0x338000			 \
		-b 0x348000			 \
		-b 0x358000			 \
		-b 0x368000			 \
		-b 0x378000			 \
		-b 0x388000			 \
		-b 0x398000			 \
		-b 0x3a8000			 \
		-b 0x3b8000			 \
		-b 0x3c8000			 \
		-b 0x3d8000			 \
		-b 0x3e8000			 \
		-b 0x3f8000
endif

CRT0 =
CRT0 += ${LIBMSX_HOME}/lib/crt0_mod_rom_header/${CONFIG_CRT0_MOD_ROM_HEADER}.rel
CRT0 += ${LIBMSX_HOME}/lib/crt0_mod_init/${CONFIG_CRT0_MOD_INIT}.rel
CRT0 += ${LIBMSX_HOME}/lib/crt0_mod_start/${CONFIG_CRT0_MOD_START}.rel
ifneq (${CONFIG_CRT0_MOD_ROM_MAPPER},)
CRT0 += ${LIBMSX_HOME}/lib/crt0_mod_rom_mapper/${CONFIG_CRT0_MOD_ROM_MAPPER}.rel
endif
CRT0 += $(patsubst %,${LIBMSX_HOME}/lib/crt0_mod_libs/%.rel,${CONFIG_CRT0_MOD_LIBS})
# ------------------------------------------------------------------------------

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin
RSCDIR ?= resources

# list of auto-generated source files
BUILT_SOURCES ?=

# -- .aks files --
AKS2C = ${LIBMSX_HOME}/bin/aks2c.sh

SRCS_AKS ?= $(shell find ${SRCDIR} -type f -name '*.aks')
SRCS_AKS_C = $(patsubst %.aks, %.c, ${SRCS_AKS})
SRCS_AKS_H = $(patsubst %.aks, %.h, ${SRCS_AKS})

BUILT_SOURCES += ${SRCS_AKS_C} ${SRCS_AKS_H}

# -- resources --
RIDXC = ${LIBMSX_HOME}/bin/ridxc.sh
RSCS_INDEX_C = ${OBJDIR}/libmsx_resources.c

ifneq (${RSCDIR},)
RSCS = $(shell find ${RSCDIR} -type f -print 2>/dev/null)
endif

# ----
SRCS_C ?= $(filter %.c, ${BUILT_SOURCES})
SRCS_C += $(shell find ${SRCDIR} -type f -name '*.c')
OBJS_C = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.rel, ${SRCS_C})

SRCS_ASM = $(shell find ${SRCDIR} -type f -name '*.s')
OBJS_ASM = $(patsubst ${SRCDIR}/%.s, ${OBJDIR}/%.rel, ${SRCS_ASM})

OBJS = ${OBJS_ASM} ${OBJS_C}
ifneq (${RSCS},)
OBJS += $(patsubst %.c, %.rel, ${RSCS_INDEX_C})
endif

DEPS = $(patsubst %.rel, %.d, $(OBJS))

TARGETS = \
	${BINDIR}/${NAME}.dat \
	${BINDIR}/${NAME}.rom

CC = sdcc
AS = sdasz80
LD = sdld
AR = sdar
OBJCOPY = sdobjcopy
IHX2BIN = ${LIBMSX_HOME}/bin/ihx2bin
PAD2POW2 = ${LIBMSX_HOME}/bin/pad2pow2.sh

CFLAGS ?=
CFLAGS += -mz80 -MMD
CFLAGS += -I ${LIBMSX_HOME}/include
CFLAGS += --codeseg APPLCODE

LIBMSX = ${LIBMSX_HOME}/lib/libmsx.lib

LIBS ?=
LIBS += ${LIBMSX}

LDFLAGS ?=
LDFLAGS += --no-std-crt0 --code-loc ${ADDR_CODE} --data-loc ${ADDR_DATA}
LDFLAGS += -L ${LIBMSX_HOME}/lib

LDLIBS ?=
# LDLIBS += -llibmsx

build: ${TARGETS}

clean:
	@rm -f ${TARGETS} ${OBJS} ${DEPS} ${BUILT_SOURCES} ${RSCS_INDEX_C}
	@rm -rf ${OBJDIR} ${BINDIR}

ifdef LIBMSX_HOME
libmsx:
	@${MAKE} -C ${LIBMSX_HOME}
else
libmsx:
	@${info LIBMSX_HOME is not defined in Makefile.}
	@${info LIBMSX_HOME shall be defined as follows:}
	@${info LIBMSX_HOME = <path to top of libmsx>}
endif

${CRT0} ${LIBMSX}: libmsx

%.bin: %.ihx
	@${info [Build]	$@}
	@${IHX2BIN} ${IHX2BIN_FLAGS} -o $@ $<
	@truncate -s %16K $@

${BINDIR}/${NAME}.rom: ${BINDIR}/${NAME}.bin ${RSCS}
	@${info [Build]	$@ <- $^}
	@cat $^ > $@ && ${PAD2POW2} $@

${BINDIR}/${NAME}.dat: ${BINDIR}/${NAME}.ihx
	@${info [Build]	$@}
	@${IHX2BIN} -o $@ $<

${RSCS_INDEX_C}: ${SRCS_C} ${RSCS}
	@${info [Build] Resources index}
	@${RIDXC} -o $@ ${RSCDIR} ${SRCS_C}

# %.rom: %.ihx
# 	@${info [Build]	$@}
# 	@${OBJCOPY} -I ihex -O binary \
# 	--gap-fill 0xff \
# 	--pad-to $$(printf '%d' $$((${ADDR_HEAD} + ${IMAGE_SIZE}))) \
# 	$< $@

# %.dat: %.ihx
# 	@${info [Build]	$@}
# 	@${OBJCOPY} -I ihex -O binary $< $@

${BINDIR}/${NAME}.ihx: ${CRT0} ${OBJS} ${LIBS}
	@${info [LD]	$@}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ ${LDFLAGS} $^ ${LDLIBS}

%.rel: %.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.rel: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.rel: ${SRCDIR}/%.s
	@${info [AS]	$<}
	@mkdir -p $(dir $@)
	@${AS} -o $@ $<

${SRCS_AKS_C}: %.c: %.aks | %.h
	@${info [AKS2C]	$< -> $@}
	@${AKS2C} --name "$(notdir $(basename $<))" -o $@ $<

${SRCS_AKS_H}: %.h: %.aks
	@${info [AKS2C]	$< -> $@}
	@${AKS2C} --name "$(notdir $(basename $<))" -o $@ $< --header

-include $(DEPS)
