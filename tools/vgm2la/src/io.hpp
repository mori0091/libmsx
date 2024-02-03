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
 * \file io.hpp
 */

#ifndef IO_HPP_
#define IO_HPP_

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

// read from input stream

inline uint8_t u8(std::istream & is) {
  return (uint8_t)is.get();
}
inline int8_t i8(std::istream & is) {
  return (int8_t)u8(is);
}
inline uint16_t u16(std::istream & is) {
  return u8(is) | (u8(is) << 8);
}
inline uint32_t u32(std::istream & is) {
  return u8(is) | (u8(is) << 8) | (u8(is) << 16) | (u8(is) << 24);
}

// read from a byte-array

inline int8_t i8(const char * p) {
  return (int8_t)*p;
}
inline uint8_t u8(const char * p) {
  return (uint8_t)*p;
}
inline uint16_t u16(const char * p) {
  return u8(p) | (u8(p+1) << 8);
}
inline uint32_t u32(const char * p) {
  return u8(p) | (u8(p+1) << 8) | (u8(p+2) << 16) | (u8(p+3) << 24);
}

// write to output stream

inline void write(std::ostream & os, int8_t x) {
  os << (char)x;
}

inline void write(std::ostream & os, uint8_t x) {
  os << (char)x;
}

inline void write(std::ostream & os, uint16_t x) {
  os << (char)(x & 0xff) << (char)((x >> 8) & 0xff);
}

inline void write(std::ostream & os, uint32_t x) {
  os << (char)(x & 0xff) << (char)((x >> 8) & 0xff) << (char)((x >> 16) & 0xff) << (char)((x >> 24) & 0xff);
}

template <class T>
void write(std::ostream & os, const std::vector<T> & xs) {
  for (auto beg = xs.begin(); beg != xs.end(); beg++) {
    write(os, *beg);
  }
}

std::ofstream file_open_w(const std::string & filename);

/**
 * Load the specified file into a vector.
 *
 * \param filename  Path to the file.
 * \return Contents of the file as `std::vector<char>`.
 */
std::vector<char> load_file(const std::string & filename);

/**
 * Load the specified (gzipped or plain) file into a vector.
 *
 * \param filename  Path to the file. (gzipped or plain)
 * \return Decompressed contents of the file as `std::vector<char>`.
 */
std::vector<char> load_gzfile(const std::string & filename);

/**
 * Wrapper for use `std::vector<char>` as `std::streambuf`.
 */
class streambuf_from_vector : public std::streambuf {
  public:
    streambuf_from_vector(std::vector<char> & v_);

  protected:
    pos_type seekpos(pos_type pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);

  private:
    std::vector<char> & v;
};

#endif // IO_HPP_
