#pragma once

#include <stdint.h>

#define SCALE 10

#define CANVAS_WIDTH (20 * SCALE)
#define CANVAS_HEIGHT (20 * SCALE)

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define S_X(x) ((CANVAS_WIDTH / 2) + x)
#define S_Y(y) ((CANVAS_HEIGHT / 2) - y)
#define XY(x, y) (S_Y(y) * CANVAS_WIDTH + S_X(x))

typedef uint32_t Color;

typedef int pixel[3];

#define RGB(r, g, b) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)0xFF)
#define RGBA(r, g, b, a) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)a)


/* #define put_pixel(fb, x, y, c) if(x >= 0 && y >= 0 && x < CANVAS_WIDTH && y < CANVAS_HEIGHT) fb[XY(x, y)] = c */
#define put_pixel(fb, x, y, c) fb[XY(x, y)] = c

void draw_line(Color* fb, pixel p0, pixel p1, Color c);
void draw_wireframe_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c);
void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c);
void draw_filled_triangle_2(Color* fb, pixel p0, pixel p1, pixel p2, Color c);


#define COLOR_BLACK               RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE               RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED                 RGB(0xFF, 0x00, 0x00)
#define COLOR_GREEN               RGB(0x00, 0xFF, 0x00)
#define COLOR_BLUE                RGB(0x00, 0x00, 0xFF)
#define COLOR_YELLOW              RGB(0xFF, 0xFF, 0x00)
#define COLOR_CYAN                RGB(0x00, 0xFF, 0xFF)
#define COLOR_MAGENTA             RGB(0xFF, 0x00, 0xFF)
#define COLOR_GRAY                RGB(0x80, 0x80, 0x80)
#define COLOR_LIGHT_GRAY          RGB(0xC0, 0xC0, 0xC0)
#define COLOR_DARK_GRAY           RGB(0x40, 0x40, 0x40)
#define COLOR_MAROON              RGB(0x80, 0x00, 0x00)
#define COLOR_DARK_GREEN          RGB(0x00, 0x80, 0x00)
#define COLOR_NAVY                RGB(0x00, 0x00, 0x80)
#define COLOR_OLIVE               RGB(0x80, 0x80, 0x00)
#define COLOR_TEAL                RGB(0x00, 0x80, 0x80)
#define COLOR_PURPLE              RGB(0x80, 0x00, 0x80)
#define COLOR_SILVER              RGB(0xC0, 0xC0, 0xC0)
#define COLOR_LIGHT_RED           RGB(0xFF, 0x66, 0x66)
#define COLOR_LIGHT_GREEN         RGB(0x66, 0xFF, 0x66)
#define COLOR_LIGHT_BLUE          RGB(0x66, 0x66, 0xFF)
#define COLOR_LIGHT_YELLOW        RGB(0xFF, 0xFF, 0x66)
#define COLOR_LIGHT_CYAN          RGB(0x66, 0xFF, 0xFF)
#define COLOR_LIGHT_MAGENTA       RGB(0xFF, 0x66, 0xFF)
#define COLOR_DARK_RED            RGB(0x80, 0x00, 0x00)
#define COLOR_DARK_GREEN          RGB(0x00, 0x80, 0x00)
#define COLOR_DARK_BLUE           RGB(0x00, 0x00, 0x80)
#define COLOR_DARK_YELLOW         RGB(0x80, 0x80, 0x00)
#define COLOR_DARK_CYAN           RGB(0x00, 0x80, 0x80)
#define COLOR_DARK_MAGENTA        RGB(0x80, 0x00, 0x80)
#define COLOR_LIGHT_ORANGE        RGB(0xFF, 0xCC, 0x99)
#define COLOR_DARK_ORANGE         RGB(0xCC, 0x66, 0x00)
#define COLOR_GOLD                RGB(0xFF, 0xD7, 0x00)
#define COLOR_SALMON              RGB(0xFA, 0x80, 0x72)
#define COLOR_SKY_BLUE            RGB(0x87, 0xCE, 0xEB)
#define COLOR_LIME                RGB(0x00, 0xFF, 0x00)
#define COLOR_INDIGO              RGB(0x4B, 0x00, 0x82)
#define COLOR_BEIGE               RGB(0xF5, 0xF5, 0xDC)
#define COLOR_TURQUOISE           RGB(0x40, 0xE0, 0xD0)
#define COLOR_AQUAMARINE          RGB(0x7F, 0xFF, 0xD4)
#define COLOR_CORAL               RGB(0xFF, 0x7F, 0x50)
#define COLOR_CRIMSON             RGB(0xDC, 0x14, 0x3C)
#define COLOR_DARK_SLATE_GRAY     RGB(0x2F, 0x4F, 0x4F)
#define COLOR_DIM_GRAY            RGB(0x69, 0x69, 0x69)
#define COLOR_FIREBRICK           RGB(0xB2, 0x22, 0x22)
#define COLOR_FOREST_GREEN        RGB(0x22, 0x8B, 0x22)
#define COLOR_INDIAN_RED          RGB(0xCD, 0x5C, 0x5C)
#define COLOR_KHAKI               RGB(0xF0, 0xE6, 0x8C)
#define COLOR_MEDIUM_AQUAMARINE   RGB(0x66, 0xCD, 0xAA)
#define COLOR_MEDIUM_ORCHID       RGB(0xBA, 0x55, 0xD3)
#define COLOR_MEDIUM_PURPLE       RGB(0x93, 0x71, 0xDB)
#define COLOR_MEDIUM_SEA_GREEN    RGB(0x3C, 0xB3, 0x71)
#define COLOR_MEDIUM_SLATE_BLUE   RGB(0x7B, 0x68, 0xEE)
#define COLOR_MEDIUM_SPRING_GREEN RGB(0x00, 0xFA, 0x9A)
#define COLOR_MEDIUM_TURQUOISE    RGB(0x48, 0xD1, 0xCC)
#define COLOR_MEDIUM_VIOLET_RED   RGB(0xC7, 0x15, 0x85)
#define COLOR_MIDNIGHT_BLUE       RGB(0x19, 0x19, 0x70)
#define COLOR_OLIVE_DRAB          RGB(0x6B, 0x8E, 0x23)
#define COLOR_ORANGE              RGB(0xFF, 0xA5, 0x00)
#define COLOR_ORCHID              RGB(0xDA, 0x70, 0xD6)
#define COLOR_PALE_GREEN          RGB(0x98, 0xFB, 0x98)
#define COLOR_PALE_TURQUOISE      RGB(0xAF, 0xEE, 0xEE)
#define COLOR_PALE_VIOLET_RED     RGB(0xDB, 0x70, 0x93)
#define COLOR_PERU                RGB(0xCD, 0x85, 0x3F)
#define COLOR_ROYAL_BLUE          RGB(0x41, 0x69, 0xE1)
#define COLOR_SADDLE_BROWN        RGB(0x8B, 0x45, 0x13)
#define COLOR_SEA_GREEN           RGB(0x2E, 0x8B, 0x57)
#define COLOR_SIENNA              RGB(0xA0, 0x52, 0x2D)
#define COLOR_SLATE_BLUE          RGB(0x6A, 0x5A, 0xCD)
#define COLOR_SLATE_GRAY          RGB(0x70, 0x80, 0x90)
#define COLOR_SPRING_GREEN        RGB(0x00, 0xFF, 0x7F)
#define COLOR_STEEL_BLUE          RGB(0x46, 0x82, 0xB4)
#define COLOR_TAN                 RGB(0xD2, 0xB4, 0x8C)
#define COLOR_THISTLE             RGB(0xD8, 0xBF, 0xD8)
#define COLOR_TOMATO              RGB(0xFF, 0x63, 0x47)
#define COLOR_VIOLET              RGB(0xEE, 0x82, 0xEE)
#define COLOR_WHEAT               RGB(0xF5, 0xDE, 0xB3)
#define COLOR_YELLOW_GREEN        RGB(0x9A, 0xCD, 0x32)
