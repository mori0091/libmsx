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
 * \file file_io.h
 */

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BUF_SIZE   (1024*1024)

extern uint8_t plaintext[BUF_SIZE];
extern uint8_t encoded[BUF_SIZE];

size_t read_file(const char * file);

size_t write_file(const char * file, const uint8_t * src, size_t size);

bool is_file_exists(const char * file);

#endif // FILE_IO_H_
