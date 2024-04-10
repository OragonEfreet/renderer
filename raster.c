#include <stdlib.h>

#include "raster.h"

#define X 0
#define Y 1

typedef struct { float a; float d; int i; int ie; } interpolate_iterator;
#define interpolate_begin(i0,d0,i1,d1) {.a=((float)((d1)-(d0)))/((float)((i1)-(i0))),.d=d0,.i=i0,.ie=i1}
#define interpolate_done(dat) (dat.i>dat.ie)
#define interpolate_next(dat) ++dat.i,dat.d+=dat.a 

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

/* void draw_filled_triangle(Color* fb, pixel v0, pixel v1, pixel v2, Color c) { */
#define SWAP_INT(a, b) a=a^b;b=a^b;a=a^b
void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {

    if(p1[Y] < p0[Y]) {SWAP_INT(p1[X], p0[X]);SWAP_INT(p1[Y], p0[Y]);}
    if(p2[Y] < p0[Y]) {SWAP_INT(p2[X], p0[X]);SWAP_INT(p2[Y], p0[Y]);}
    if(p2[Y] < p1[Y]) {SWAP_INT(p2[X], p1[X]);SWAP_INT(p2[Y], p1[Y]);}
    
    int total_height = p2[Y] - p0[Y];

    // Drawing bottom part of triangle
    int segment_height = p2[Y] - p1[Y] + 1;
    for (int y = p1[Y] + 1; y <= p2[Y]; y++) {

        float alpha = (float)(y - p0[Y]) / total_height;
        float beta = (float)(y - p1[Y]) / segment_height;

        int ax = p0[X] + (p2[X] - p0[X]) * alpha;
        int bx = p1[X] + (p2[X] - p1[X]) * beta;

        if (ax > bx) { SWAP_INT(ax, bx); }

        for (int x = ax; x <= bx; x++) {
            put_pixel(fb, x, y, c);
        }
    }

    // Drawing upper part of triangle
    segment_height = p1[Y] - p0[Y] + 1;
    for (int y = p0[Y]; y <= p1[Y]; y++) {

        float alpha = (float)(y - p0[Y]) / total_height;
        float beta = (float)(y - p0[Y]) / segment_height;

        int ax = p0[X] + (p2[X] - p0[X]) * alpha;
        int bx = p0[X] + (p1[X] - p0[X]) * beta;

        if (ax > bx) { SWAP_INT(ax, bx); }

        for (int x = ax; x <= bx; x++) {
            put_pixel(fb, x, y, c);
        }
    }
}
#undef SWAP_INT

     
