// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file zx0.c
 *
 * ZX0 optimal file compressor with several extended features.
 * Compatible with ZX0 data compression format v2.
 *
 * The ZX0 data compression format and algorithm was designed by Einar Saukas.
 *
 * \sa https://github.com/einar-saukas/ZX0
 *
 * This C implementation of the ZX0 compressor has been reimplemented by Daishi
 * Mori (mori0091).
 */

#include "zx0.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "file_io.h"
#include "info.h"
#include "options.h"

int main(int argc, char ** argv) {
  argc = parse_args(argc, argv);
  if (argc == 1) {
    help();
  }
  DEBUG_INFO("force=%d\n", (int)opt_force);
  DEBUG_INFO("suffix=%s\n", opt_suffix);
  DEBUG_INFO("skip=%zu\n", opt_skip);
  DEBUG_INFO("align=%zu\n", opt_align);
  DEBUG_INFO("bsave=%d\n", (int)opt_bsave);
  for (int i = 1; i < argc; ++i) {
    DEBUG_INFO("%s ", argv[i]);
  }
  DEBUG_INFO("\n");

  for (int i = 1; i < argc; ++i) {
    const char * input = argv[i];
    char * output = array(strlen(input) + strlen(opt_suffix) + 1, 1);
    strcpy(output, input);
    strcat(output, opt_suffix);

    if (!opt_force && is_file_exists(output)) {
      ERROR("File already exists: '%s'", output);
    }

    const size_t input_length = read_file(input);
    if (input_length <= opt_skip) {
      ERROR("Input file is too short: `%s`", input);
    }
    const size_t plaintext_length = input_length - opt_skip;
    INFO("%s: %zu bytes\n", input, input_length);
    if (opt_skip) {
      INFO("- skip %zu bytes\n", opt_skip);
    }
    INFO("- compress %zu bytes\n", plaintext_length);
    if (opt_align) {
      INFO("- align to %zu bytes boundary\n", opt_align);
    }
    if (opt_bsave) {
      assert(opt_skip == 7);
      const uint16_t beg = (uint16_t)plaintext[2] << 8 | plaintext[1];
      const uint16_t end = (uint16_t)plaintext[4] << 8 | plaintext[3];
      const size_t len = (end - beg + 1);
      if (plaintext[0] != 0xfe || len != input_length - 7) {
        ERROR("Not a MSX BSAVE format file: '%s'", input);
      }
      INFO("- address=[%04X..%04X]\n", (int)beg, (int)end);
    }

    const size_t encoded_length = compress(plaintext + opt_skip, plaintext_length);
    write_file(output, encoded, encoded_length);
    free(output);
    INFO("File compressed from %zu to %zu bytes.\n", plaintext_length, encoded_length);
    INFO("\n");
  }

  return 0;
}
