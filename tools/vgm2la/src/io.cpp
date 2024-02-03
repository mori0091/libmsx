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
 * \file io.cpp
 */

#include "io.hpp"

std::ofstream file_open_w(const std::string & filename) {
  std::ofstream os{filename, std::ios_base::binary};
  if (os.fail()) {
    std::cerr << "Cannot open file to write - \"" << filename << "\"" << std::endl;
    exit(1);
  }
  return os;
}

std::vector<char> load_file(const std::string & filename) {
  std::ifstream is(filename, std::ios_base::binary);
  if (is.fail()) {
    std::cerr << "Cannot open file '" << filename << "'." << std::endl;
    exit(1);
  }
  std::istreambuf_iterator<char> it(is);
  std::istreambuf_iterator<char> end;
  return std::vector<char>(it, end);
}

#include <zlib.h>

std::vector<char> load_gzfile(const std::string & filename) {
  auto fp = gzopen(filename.c_str(), "rb");
  if (!fp) {
    std::cerr << "Cannot open file '" << filename << "'." << std::endl;
    exit(1);
  }
  std::vector<char> ret;
  ret.reserve(1024 * 1024);
  int c;
  while (0 <= (c = gzgetc(fp))) {
    ret.push_back((char)c);
  }
  ret.shrink_to_fit();
  gzclose(fp);
  return ret;
}

streambuf_from_vector::streambuf_from_vector(std::vector<char> & v_) : v(v_) {
  this->setg(v.data(), v.data(), v.data() + v.size());
  this->setp(v.data(), v.data() + v.size());
}

std::streambuf::pos_type streambuf_from_vector::seekpos(pos_type pos, std::ios_base::openmode which) {
  if (pos < 0 || v.size() < pos || !(which & (std::ios_base::in | std::ios_base::out))) {
    return pos_type(off_type(-1));
  }
  if (which & std::ios_base::in) {
    this->setg(v.data(), v.data() + pos, v.data() + v.size());
  }
  if (which & std::ios_base::out) {
    this->setp(v.data() + pos, v.data() + v.size());
  }
  return pos;
}
