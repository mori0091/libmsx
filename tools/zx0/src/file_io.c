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
 * \file file_io.c
 */

#include "file_io.h"

#include <stdio.h>

#include "info.h"

uint8_t plaintext[BUF_SIZE];
uint8_t encoded[BUF_SIZE];

size_t read_file(const char * file) {
  FILE * fp = fopen(file, "rb");
  if (!fp) {
    ERROR("cannot open file to read");
  }
  size_t len = 0;
  while (!feof(fp)) {
    size_t n = fread(plaintext + len, 1, sizeof(plaintext) - len, fp);
    if (!n) {
      ERROR("failed to read file");
    }
    len += n;
  }
  fclose(fp);
  return len;
}

size_t write_file(const char * file, const uint8_t * src, size_t size) {
  FILE * fp = fopen(file, "wb");
  if (!fp) {
    ERROR("cannot open file to write");
  }
  size_t len = 0;
  while (len < size) {
    size_t n = fwrite(src + len, 1, size - len, fp);
    if (!n) {
      ERROR("failed to write file");
    }
    len += n;
  }
  fclose(fp);
  return len;
}

bool is_file_exists(const char * file) {
  FILE * fp = fopen(file, "rb");
  if (fp) {
    fclose(fp);
    return true;
  }
  return false;
}
