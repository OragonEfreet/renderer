#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raster.h"

#define X 0
#define Y 1
/* #define put_pixel(x, y, c) framebuffer[XY(x, y)] = c; */

typedef struct { float a; float d; float i; float ie; } pixel_interpolate;
#define start_interpolate(i0,d0,i1,d1) {.a=((d1)-(d0))/((i1)-(i0)),.d=d0,.i=i0,.ie=i1}
#define done_interpolate(dat) (dat.i>dat.ie)
#define step_interpolate(dat) ++dat.i,dat.d+=dat.a 

void draw_line(Color* fb, pixel p0, pixel p1, Color c) {
    int x0 = p0[X]; int y0 = p0[Y];
    const int x1 = p1[X]; const int y1 = p1[Y];

    const int dx = abs(x1 - x0);
    const int dy = abs(y1 - y0);
    const int sx = (x0 < x1) ? 1 : -1;
    const int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        put_pixel(fb, x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        const int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}


void draw_wireframe_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {
    draw_line(fb, p0, p1, c);
    draw_line(fb, p1, p2, c);
    draw_line(fb, p2, p0, c);
}

void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {
    // Sorting vertices vertically
    if (p0[1] > p1[1]) {
        int temp_x = p0[0];
        int temp_y = p0[1];
        p0[0] = p1[0];
        p0[1] = p1[1];
        p1[0] = temp_x;
        p1[1] = temp_y;
    }
    if (p0[1] > p2[1]) {
        int temp_x = p0[0];
        int temp_y = p0[1];
        p0[0] = p2[0];
        p0[1] = p2[1];
        p2[0] = temp_x;
        p2[1] = temp_y;
    }
    if (p1[1] > p2[1]) {
        int temp_x = p1[0];
        int temp_y = p1[1];
        p1[0] = p2[0];
        p1[1] = p2[1];
        p2[0] = temp_x;
        p2[1] = temp_y;
    }

    int total_height = p2[1] - p0[1];

    // Drawing upper part of triangle
    for (int y = p0[1]; y <= p1[1]; y++) {
        int segment_height = p1[1] - p0[1] + 1;
        int alpha = (y - p0[1]) * 255 / total_height;
        int beta = (y - p0[1]) * 255 / segment_height;

        int ax = p0[0] + ((p2[0] - p0[0]) * alpha + 128) / 255;
        int bx = p0[0] + ((p1[0] - p0[0]) * beta + 128) / 255;

        if (ax > bx) {
            int temp = ax;
            ax = bx;
            bx = temp;
        }

        for (int x = ax; x <= bx; x++) {
            put_pixel(fb, x, y, c);
        }
    }

    // Drawing bottom part of triangle
    for (int y = p1[1] + 1; y <= p2[1]; y++) {
        int segment_height = p2[1] - p1[1] + 1;
        int alpha = (y - p0[1]) * 255 / total_height;
        int beta = (y - p1[1]) * 255 / segment_height;

        int ax = p0[0] + ((p2[0] - p0[0]) * alpha + 128) / 255;
        int bx = p1[0] + ((p2[0] - p1[0]) * beta + 128) / 255;

        if (ax > bx) {
            int temp = ax;
            ax = bx;
            bx = temp;
        }

        for (int x = ax; x <= bx; x++) {
            put_pixel(fb, x, y, c);
        }
    }
}

     
