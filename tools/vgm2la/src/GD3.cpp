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
 * \file GD3.cpp
 */

#include "GD3.hpp"

void GD3::print() {
  std::cout << "GD3 tags : " << std::endl;
  std::cout << "  Track name (en)     : " << track_name_en  << std::endl;
  std::cout << "  Game name (en)      : " << game_name_en   << std::endl;
  std::cout << "  System name (en)    : " << system_name_en << std::endl;
  std::cout << "  Composer (en)       : " << composer_en    << std::endl;
  std::cout << "  Game release date   : " << date           << std::endl;
  std::cout << "  VGM data created by : " << created_by     << std::endl;
  std::cout << "  Notes               : " << notes          << std::endl;
}

static std::wstring read_GD3_tag(std::istream & is) {
  std::wstring s;
  uint16_t c;
  while ((c = u16(is))) {
    s += (wchar_t)c;
  }
  return s;
}

static std::string read_GD3_tag_en(std::istream & is) {
  std::string s;
  uint16_t c;
  while ((c = u16(is))) {
    if (c < 256 && std::isprint((char)c, std::locale::classic())) {
      s += (char)c;
    }
    else {
      s += "_";
    }
  }
  return s;
}

void GD3::parse(std::istream & is) {
  char id[4] = {0};
  is.read(id, 4);
  if (strncmp(id, "Gd3 ", 4)) {
    std::cerr << "GD3 tags not found." << std::endl;
    exit(1);
  }
  version = u32(is);
  if (0x100 != version) {
    std::cerr << "Unsupported GD3 version - '0x" << std::hex << version << std::dec << "'." << std::endl;
    return;
  }
  auto length = u32(is);
  track_name_en  = read_GD3_tag_en(is);
  track_name     = read_GD3_tag(is); // unknown multi-byte character encoding
  game_name_en   = read_GD3_tag_en(is);
  game_name      = read_GD3_tag(is); // unknown multi-byte character encoding
  system_name_en = read_GD3_tag_en(is);
  system_name    = read_GD3_tag(is); // unknown multi-byte character encoding
  composer_en    = read_GD3_tag_en(is);
  composer       = read_GD3_tag(is); // unknown multi-byte character encoding
  date           = read_GD3_tag_en(is);
  created_by     = read_GD3_tag_en(is);
  notes          = read_GD3_tag_en(is);
}
