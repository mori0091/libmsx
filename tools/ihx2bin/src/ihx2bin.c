// -*- coding: utf-8-unix -*-
/**
 * \file ihx2bin.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAD_BYTE (0x0ff)

static uint8_t image[4 * 1024 * 1024]; // 4 MiB
static uint8_t csum = 0;
static int line = 1;

static char* out_file = 0;
static int nblocks = 0;
static struct block {
  uint32_t size;
  uint32_t base;
} blocks[256];

static bool pad_to_pow2 = false;

uint32_t round_up_to_pow2(uint32_t x) {
  /* assert(x < (1 << 31)); */
  if (!x || 0x80000000u <= x) {
    return x;
  }
  uint32_t y = 1;
  for (int i = 0; i < 32 && y < x; ++i, y <<= 1)
    ;
  return y;
}

FILE* open_file(const char* file, const char* mode) {
  FILE* f = fopen(file, mode);
  if (!f) {
    fprintf(stderr, "cannot open file -- '%s' (mode: %s)\n", file, mode);
    exit(1);
  }
  return f;
}

void close_file(FILE* f) {
  if (f) {
    fclose(f);
  }
}

int get_char(FILE* in) {
  int c = fgetc(in);
  if (c == '\n') {
    line++;
  }
  return c;
}

void unget_char(int c, FILE* in) {
  if (c == '\n') {
    line--;
  }
  c = ungetc(c, in);
  if (c == EOF) {
    perror("ungetc");
    exit(1);
  }
}

void line_number(void) {
  fprintf(stderr, "at line %d\n", line);
}

_Noreturn
void unexpected_eof(void) {
  line_number();
  fprintf(stderr, "unexpected end of file\n");
  exit(1);
}

_Noreturn
void unexpected_char(int c, const char* info) {
  line_number();
  if (c == ' ' || isgraph(c)) {
    fprintf(stderr, "unexpected '%c'\n", c);
  } else {
    fprintf(stderr, "unexpected char (0x%02X)\n", c);
  }
  if (info) {
    fprintf(stderr, "expecting %s\n", info);
  }
  exit(1);
}

int any_char(FILE* in) {
  int c = get_char(in);
  if (c == EOF) {
    unexpected_eof();
  }
  return c;
}

char char1(FILE* in, int ch) {
  int c = any_char(in);
  if (c != ch) {
    char info[8];
    if (ch == ' ' || isgraph(ch)) {
      snprintf(info, sizeof(info), "'%c'", ch);
    } else {
      snprintf(info, sizeof(info), "(0x%2X)", ch);
    }
    unexpected_char(c, info);
  }
  return c;
}

void skip_any_chars_not_of(FILE* in, int ch) {
  for (;;) {
    int c = get_char(in);
    if (c == EOF) {
      return;
    }
    if (c != ch) {
      continue;
    }
    unget_char(c, in);
    return;
  }
}

int hex_digit(FILE* in) {
  int c = any_char(in);
  if ('0' <= c && c <= '9') {
    return (c - '0');
  }
  if ('a' <= c && c <= 'f') {
    return (c - 'a' + 10);
  }
  if ('A' <= c && c <= 'F') {
    return (c - 'A' + 10);
  }
  /* hex character not found */
  unexpected_char(c, "a hex digit");
}

uint8_t hex_1byte(FILE* in) {
  uint8_t x = (uint8_t)(hex_digit(in) << 4 | hex_digit(in));
  csum += x;
  return x;
}

uint16_t hex_2byte(FILE* in) {
  return (uint16_t)(hex_1byte(in) << 8 | hex_1byte(in));
}

uint32_t hex_4byte(FILE* in) {
  return (uint32_t)(hex_2byte(in) << 16 | hex_2byte(in));
}

void hex_nbytes(FILE * in, void * dst, size_t n) {
  uint8_t * p = (uint8_t *)dst;
  while (n--) {
    *p++ = hex_1byte(in);
  }
}

void checksum(FILE* in) {
  hex_1byte(in);
  if (csum) {
    line_number();
    fprintf(stderr, "checksum error\n");
    exit(1);
  }
}

enum record_type {
  DATA                     = 0,
  END_OF_FILE,
  EXTENDED_SEGMENT_ADDRESS,
  SEGMENT_ADDRESS,
  EXTENDED_ADDRESS,
  LINEAR_32BIT_ADDRESS,
};

#define min(a,b) ((a) < (b) ? (a) : (b));
#define max(a,b) ((a) > (b) ? (a) : (b));

void compile(FILE* in, FILE* out) {
  uint32_t addr_min = sizeof(image);
  uint32_t addr_max = 0;
  uint32_t offset = 0;
  bool end = false;
  csum = 0;
  line = 1;
  while (!end) {
    skip_any_chars_not_of(in, ':');
    char1(in, ':');
    uint8_t count = hex_1byte(in);
    uint16_t address = hex_2byte(in);
    uint8_t record_type = hex_1byte(in);
    switch (record_type) {
      case DATA:
        offset = (offset & 0xffff0000) | address;
        if (sizeof(image) <= offset + count) {
          fprintf(stderr, "out of bounds (Too large image)\n");
          exit(1);
        }
        hex_nbytes(in, &image[offset], count);
        break;
      case END_OF_FILE:
        end = true;
        break;
      case EXTENDED_ADDRESS:
        if (count != 2) {
          line_number();
          fprintf(stderr, "unexpected byte count %2X\n", (int)count);
          fprintf(stderr, "expecting byte count = 02 for record type 04\n");
          exit(1);
        }
        uint16_t hi_addr = hex_2byte(in);
        offset = ((uint32_t)hi_addr << 16) | (offset & 0x0000ffff);
        break;
      case LINEAR_32BIT_ADDRESS:
      case EXTENDED_SEGMENT_ADDRESS:
      case SEGMENT_ADDRESS:
        line_number();
        fprintf(stderr, "unsupported record type - %2X\n", (int)record_type);
        exit(1);
        break;
      default:
        line_number();
        fprintf(stderr, "unknown record type - %2X\n", (int)record_type);
        exit(1);
        break;
    }
    checksum(in);
    addr_min = min(offset, addr_min);
    offset += count;
    addr_max = max(offset, addr_max);
  }
  skip_any_chars_not_of(in, ':');
  if (!feof(in)) {
    line_number();
    fprintf(stderr, "unexpected octet found after END OF FILE (01) record\n");
    exit(1);
  }
  // ----
  if (addr_min >= addr_max) {
    fprintf(stderr, "no data\n");
    exit(1);
  }
  fprintf(stderr, "address: [%08X..%08X]\n", addr_min, addr_max-1);
  if (!nblocks) {
    blocks[0].base = addr_min;
    blocks[0].size = addr_max - addr_min;
    nblocks = 1;
  }
  // ---- extract specified blocks
  uint32_t dst = 0;
  for (int i = 0; i < nblocks; ++i) {
    if (addr_max <= blocks[i].base) {
      // omit a block out of range
      continue;
    }
    if (!blocks[i].size) {
      blocks[i].size = addr_max - blocks[i].base;
    }
    if (blocks[i].base + blocks[i].size <= addr_min) {
      // omit a block out of range
      continue;
    }
    fwrite(image + blocks[i].base, 1, blocks[i].size, out);
    fprintf(stderr, "block #%d: %08X..%08X (%d bytes) -> %08X..%08X\n",
            i,
            (int)blocks[i].base, (int)blocks[i].base + blocks[i].size - 1, (int)blocks[i].size,
            dst, dst + blocks[i].size - 1);
    dst += blocks[i].size;
  }
  // ---- trailing padding
  if (pad_to_pow2) {
    const uint32_t pad_size = round_up_to_pow2(dst) - dst;
    if (pad_size) {
      for (uint32_t i = 0; i < pad_size; ++i) {
        fputc(PAD_BYTE, out);
      }
      fprintf(stderr, "padding:                     (%d bytes) -> %08X..%08X\n",
              (int)pad_size,
              dst, dst + pad_size - 1);
      dst += pad_size;
    }
  }
}

_Noreturn
void error_required_argument(char** p) {
  fprintf(stderr, "requires an argument -- '%s'\n", p[0]);
  exit(1);
}

_Noreturn
void error_invalid_argument(char** p) {
  fprintf(stderr, "invalid argument -- '%s %s'\n", p[0], p[1]);
  exit(1);
}

_Noreturn
void error_unknown_option(char** p) {
  fprintf(stderr, "unknown option -- '%s'\n", p[0]);
  exit(1);
}

_Noreturn
void help(void) {
  printf("Intel HEX to Binary converter\n");
  printf("\n");
  printf("Usage: ihx2bin [OPTION] [[-s INT] -b INT]... FILE\n");
  printf("\n");
  printf("Options:\n");
  printf("  -h, --help     Display this help and exit\n");
  printf("  -o FILE        output filename\n");
  printf("  -s INT         size of blocks applied to subsequent \"-b\" options\n");
  printf("  -b INT         base address of next block\n");
  printf("     --pow2      Set output file size to a power of 2");
  printf("\n");
  printf("Example:\n");
  printf("\n");
  printf("  ihx2bin -o foo.bin foo.ihx\n");
  printf("    Extracts all data records of foo.ihx\n");
  printf("    Leading no data area is skipped.\n");
  printf("    Trailing no data area is skipped.\n");
  printf("\n");
  printf("  ihx2bin -b 0x4000 -o foo.bin foo.ihx\n");
  printf("    Extracts from 0x4000\n");
  printf("    Trailing no data area is skipped.\n");
  printf("\n");
  printf("  ihx2bin -s 32768 -b 0x4000 -o foo.bin foo.ihx\n");
  printf("    Extracts 32KiB from 0x4000\n");
  printf("\n");
  printf("  ihx2bin -s 32768 -b 0x4000 -s 16384 -b 0x18000 -b 0x28000 -o foo.bin foo.ihx\n");
  printf("    Extracts 32KiB from 0x4000,\n");
  printf("             16KiB from 0x18000,\n");
  printf("             16KiB from 0x28000\n");
  printf("\n");
  printf("  Note: a block completely out of range is omitted.\n");
  printf("\n");
  exit(0);
}

static int parse_options(int argc, char** argv) {
  int32_t block_size = 0;       // current block size; 0 means unspecified
  int nargs = 0;
  char** p = argv + 1;
  char** q = argv + 1;
  for (int i = 1; i < argc; ++i) {
    if (!*p) break;
    if (**p != '-') {
      *q++ = *p++;
      nargs++;
      continue;
    }
    if (strcmp(*p, "-h") == 0 || strcmp(*p, "--help") == 0) {
      help();
    }
    if (strcmp(*p, "-o") == 0) {
      if (!p[1]) {
        error_required_argument(p);
      }
      if (p[1][0] == '-' || p[1][0] == '\0') {
        error_invalid_argument(p);
      }
      out_file = p[1];
      p += 2;
      continue;
    }
    if (strcmp(*p, "-s") == 0) {
      if (!p[1]) {
        error_required_argument(p);
      }
      if (sscanf(p[1], "%i", &block_size) < 1 || block_size <= 0) {
        error_invalid_argument(p);
      }
      p += 2;
      continue;
    }
    if (strcmp(*p, "-b") == 0) {
      if (!p[1]) {
        error_required_argument(p);
      }
      int32_t base;
      if (sscanf(p[1], "%i", &base) < 1 || base < 0) {
        error_invalid_argument(p);
      }
      blocks[nblocks].size = block_size;
      blocks[nblocks].base = base;
      nblocks++;
      p += 2;
      continue;
    }
    if (strcmp(*p, "--pow2") == 0) {
      pad_to_pow2 = true;
      p++;
      continue;
    }
    error_unknown_option(p);
  }
  return 1 + nargs;
}

int main(int argc, char** argv) {
  FILE* in;
  FILE* out = stdout;
  // display help if no arguments were given
  if (argc <= 1) {
    help();
  }
  // parse command line options
  argc = parse_options(argc, argv);
  //
  if (argc <= 1) {
    fprintf(stderr, "no input files\n");
    fprintf(stderr, "compilation terminated.\n");
    exit(1);
  }
  if (2 < argc) {
    fprintf(stderr, "too much input files\n");
    fprintf(stderr, "compilation terminated.\n");
    exit(1);
  }
  if (out_file) {
    out = open_file(out_file, "w+b");
  }

  // fill
  memset(image, PAD_BYTE, sizeof(image));

  in = open_file(argv[1], "rb");
  compile(in, out);
  close_file(in);

  close_file(out);
  return 0;
}
