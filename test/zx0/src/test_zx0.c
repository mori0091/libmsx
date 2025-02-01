// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file test_zx0.c
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <ZX0_decompress.h>

#define MAX_SIZE  (1024 * 1024)
static uint8_t plaintext[MAX_SIZE];
static uint8_t encoded[MAX_SIZE];
static uint8_t decoded[MAX_SIZE];

void error(const char * msg) {
  if (errno) {
    perror(msg);
  }
  else {
    fprintf(stderr, "%s\n", msg);
  }
  exit(1);
}

size_t read_file(const char * file, uint8_t * dst) {
  FILE * fp = fopen(file, "rb");
  if (!fp) {
    error("cannot open");
  }
  size_t len = 0;
  for (;;) {
    size_t s = fread(dst, 1, MAX_SIZE - len, fp);
    if (ferror(fp)) {
      error("read failed");
    }
    if (!s) {
      break;
    }
    dst += s;
    len += s;
  }
  fclose(fp);
  printf("%-16s : %6zu bytes\n", file, len);
  return len;
}

int main(int argc, char ** argv) {
  if (argc < 3) {
    error("requires plaintext and encoded files");
  }
  size_t plaintext_size = read_file(argv[1], plaintext);
  size_t encoded_size = read_file(argv[2], encoded);
  size_t decoded_size = ZX0_decompress(encoded, decoded);

  bool failed = false;
  printf("Decoded %zu bytes encoded message into %zu bytes plaintext.\n",
         encoded_size, decoded_size);
  if (decoded_size != plaintext_size) {
    failed = true;
    printf("Failed to decoding.\n"
           "  decoded length mismatch.\n"
           "  expecting %zu, but was %zu.\n",
           plaintext_size, decoded_size);
  }

  // --- compare plaintext and decoded message ---
  for (size_t i = 0; i < plaintext_size; ++i) {
    if (plaintext[i] != decoded[i]) {
      failed = true;
      printf("Failed at %zu\n"
             "  expecting '%02X', but was '%02X'\n",
             i, plaintext[i], decoded[i]);
    }
  }

  if (!failed) {
    printf("Succeeded. 100%% match.\n");
  }
  printf("\n");

  return failed ? 1 : 0;
}
