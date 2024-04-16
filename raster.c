#include <stdbool.h>
#include <stdlib.h>

#include <math.h>
#include <assert.h>

#include <stdio.h>

#include "raster.h"

#define X 0
#define Y 1
#define H 2

void draw_line(Color* fb, pixel p0, pixel p1, Color c) {
    /// Bresenham's line algorithm
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

typedef struct {
    float a;
    float d;
} interp;

static inline interp interp_begin(int i0, float d0, int i1, float d1) {
    return (interp) {
        .a = (d1 - d0) / ((float)i1 - (float)i0),
        .d = d0
    };
}

static inline float interp_step(interp* i) {
    return i->d += i->a;
}

/* #define interp_a(i0,d0,i1,d1) ((float)((d1)-(d0)))/((float)((i1)-(i0))) */
/* #define interp_begin(F, i0,d0,i1,d1) float F ## _a = interp_a(i0,d0,i1,d1), F ## _d = d0 */


#define SWAP_INT(a, b) a=a^b;b=a^b;a=a^b
#define SWAP_PXL(a, b) SWAP_INT(a[X], b[X]);SWAP_INT(a[Y], b[Y])
void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {
    // Sorts p0 < p1 < p2
    if(p1[Y] < p0[Y]) {SWAP_PXL(p1, p0);}
    if(p2[Y] < p0[Y]) {SWAP_PXL(p2, p0);}
    if(p2[Y] < p1[Y]) {SWAP_PXL(p2, p1);}

    // Make interpolation variables for the triangle edges
    interp x01 = interp_begin(p0[Y], p0[X], p1[Y], p1[X]);
    interp h01 = interp_begin(p0[Y], ((float)p0[H]) / 255.f, p1[Y], ((float)p1[H]) / 255.f);
    interp x12 = interp_begin(p1[Y], p1[X], p2[Y], p2[X]);
    interp h12 = interp_begin(p1[Y], ((float)p1[H]) / 255.f, p2[Y], ((float)p2[H]) / 255.f);
    interp x02 = interp_begin(p0[Y], p0[X], p2[Y], p2[X]);
    interp h02 = interp_begin(p0[Y], ((float)p0[H]) / 255.f, p2[Y], ((float)p2[H]) / 255.f);

    for(int y = p0[Y] ; y < p1[Y] ; ++y) {
        int x_left = x01.d;
        int x_right = x02.d;

        if (x_right < x_left) {SWAP_INT(x_right, x_left);}

        for(int x = x_left ; x <= x_right; ++x) {
            put_pixel(fb, x, y, c);
        }

        interp_step(&x01);
        interp_step(&x02);
    }

    for(int y = p1[Y] ; y <= p2[Y] ; ++y) {
        int x_left = x12.d;
        int x_right = x02.d;

        if (x_right < x_left) {SWAP_INT(x_right, x_left);}

        for(int x = x_left ; x <= x_right; ++x) {
            put_pixel(fb, x, y, c);
        }

        interp_step(&x12);
        interp_step(&x02);
    }
}
#undef SWAP_PXL
#undef SWAP_INT


