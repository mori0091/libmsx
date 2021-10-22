// -*- coding: utf-8-unix -*-

#ifndef SPRITE_H
#define SPRITE_H

#pragma once

#include <stdint.h>

// ---- Sprite

enum sprite_tag {
  SPRITE_TAG_IC = (1<<5),       // IC bit (Ignore Collision)
  SPRITE_TAG_CC = (1<<6),       // CC bit (Compose Color / Cancel Collision)
  SPRITE_TAG_EC = (1<<7),       // EC bit (Early Clock)
};

typedef uint8_t tagged_color_t;

// element of sprite attribute table
struct sprite {
  uint8_t y;
  uint8_t x;
  uint8_t pat;                  // sprite pattern number
  tagged_color_t tagged_color;  // EC | CC | IC | 0 | Color Code
};

// element of sprite color table
struct sprite_color {
  tagged_color_t tagged_colors[16]; // EC | CC | IC | 0 | Color Code
};

void sprite_set_x(struct sprite* s, int x);
void sprite_set_y(struct sprite* s, int y);
void sprite_set_xy(struct sprite* s, int x, int y);
void sprite_set_pat(struct sprite* s, uint8_t pat);
void sprite_set_color(struct sprite* s, tagged_color_t tagged_color);

#endif
