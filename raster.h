#pragma once

#include <stdint.h>

#include "linmath.h"

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 800

#define SCREEN_WIDTH (CANVAS_WIDTH * 1)
#define SCREEN_HEIGHT (CANVAS_HEIGHT * 1)

typedef uint32_t Color;

#define RGB(r, g, b) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)0xFF)
#define RGBA(r, g, b, a) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)a)

#define COLOR_WHITE RGB(255, 255, 255)

void put_pixel(Color* fb, int x, int y, Color c);
void draw_line(Color* fb, vec2 p0, vec2 p1, Color c);
void draw_wireframe_triangle(Color* fb, vec2 p0, vec2 p1, vec2 p2, Color c);
void draw_filled_triangle(Color* fb, vec2 p0, vec2 p1, vec2 p2, Color c);

