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
 * \file GD3.hpp
 */

#ifndef GD3_HPP_
#define GD3_HPP_

#include <cstdint>
#include <cstring>
#include <iostream>

#include "io.hpp"

struct GD3 {
  void print();
  void parse(std::istream & is);

  uint32_t version;
  std::string track_name_en;
  std::wstring track_name;
  std::string game_name_en;
  std::wstring game_name;
  std::string system_name_en;
  std::wstring system_name;
  std::string composer_en;
  std::wstring composer;
  std::string date;
  std::string created_by;
  std::string notes;
};

#endif // GD3_HPP_
