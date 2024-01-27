// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file options.h
 */

#include "options.h"

#include "array.h"
#include "info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------

bool opt_force = false;
const char * opt_suffix = ".zx0";
size_t opt_skip = 0;
size_t opt_align = 0;
bool opt_bsave = false;

void help(void) {
  INFO("Usage: zx0 [OPTION]... [FILE]...\n");
  INFO("Compress FILEs in the .zx0 format.\n");
  INFO("\n");
  INFO("Mandatory arguments to long options are mandatory for short options too.\n");
  INFO("\n");
  INFO("  -h, --help        give this help\n");
  INFO("  -f, --force       force overwrite of output files\n");
  INFO("  -S, --suffix=SUF  use suffix SUF on compressed files\n");
  INFO("\n");
  INFO("Options only for compression:\n");
  INFO("      --skip=N      ignore the first N bytes during compression\n");
  INFO("      --align=N     optimize for each N-byte aligned block during compression\n");
  INFO("      --bsave       assume input files are in MSX BSAVE format.\n");
  INFO("                    (enforces format check and '--skip=7 --align=16K')\n");
  INFO("\n");
  INFO("N may be followed by the following multiplicative suffixes:\n");
  INFO("kB=1000, K=1024, KiB=1024.\n");
  INFO("\n");
  INFO("If the option `--bsave` is specified, checks if the input file is in MSX BSAVE format.\n");
  INFO("- If so, it skips the 7-byte BSAVE header and compresses the contents body \n");
  INFO("  by MSX-optimized compression (that enables slightly faster decompression into VRAM).\n");
  INFO("- If not, report an error.\n");
  INFO("\n");
  exit(0);
}

static size_t parse_size(const char * arg) {
  size_t n = 0;
  char suf[3+1+1] = {0};
  if (1 < sscanf(arg, "%zu%4s", &n, suf)) {
    if (!strcmp(suf, "kB")) {
      n *= 1000;
    }
    else if (!strcmp(suf, "K") || !strcmp(suf, "KiB")) {
      n *= 1024;
    }
    else {
      n = 0;
    }
  }
  if (!n) {
    ERROR("Invalid argument N: '%s'", arg);
  }
  return n;
}

int parse_args(int argc, char ** argv) {
  int ret = 1;
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      help();
    }
    else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--force")) {
      opt_force = true;
    }
    else if (!strcmp(argv[i], "-S")) {
      if (argc <= i+1) {
        ERROR("Requires argument: '-S'");
      }
      opt_suffix = argv[++i];
    }
    else if (!strncmp(argv[i], "--suffix=", 9)) {
      opt_suffix = argv[i] + 9;
    }
    else if (!strncmp(argv[i], "--skip=", 7)) {
      opt_skip = parse_size(argv[i] + 7);
    }
    else if (!strncmp(argv[i], "--align=", 8)) {
      opt_align = parse_size(argv[i] + 8);
    }
    else if (!strcmp(argv[i], "--bsave")) {
      opt_skip = 7;
      opt_align = 16 * 1024;
      opt_bsave = true;
    }
    else if (argv[i][0] == '-') {
      ERROR("Illegal option: '%s'", argv[i]);
    }
    else {
      argv[ret++] = argv[i];
    }
  }
  return ret;
}
