#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "raster.h"

#define X 0
#define Y 1
#define S_X(x) ((CANVAS_WIDTH / 2) + x)
#define S_Y(y) ((CANVAS_HEIGHT / 2) - y)
#define XY(x, y) (S_Y(y) * CANVAS_WIDTH + S_X(x))
/* #define put_pixel(x, y, c) framebuffer[XY(x, y)] = c; */

typedef struct { float a; float d; float i; float ie; } pixel_interpolate;
#define start_interpolate(i0,d0,i1,d1) {.a=((d1)-(d0))/((i1)-(i0)),.d=d0,.i=i0,.ie=i1}
#define done_interpolate(dat) (dat.i>dat.ie)
#define step_interpolate(dat) ++dat.i,dat.d+=dat.a 

void put_pixel(Color* fb, int x, int y, Color c) {
    int offset = XY(x, y);
    assert(offset >= 0 && offset < (CANVAS_WIDTH * CANVAS_HEIGHT));
    if(offset >= 0 && offset < (CANVAS_WIDTH * CANVAS_HEIGHT)) {
        fb[offset] = c;
    } else {
        printf("Out of bound (%d, %d)\n", x, y);
    }
}

void draw_line(Color* fb, vec2 p0, vec2 p1, Color c) {
    float dx = p1[X] - p0[X];
    float dy = p1[Y] - p0[Y];
    int steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    float x = p0[X];
    float y = p0[Y];
    for(int i = 0; i <= steps; ++i) {
        put_pixel(fb, (int)x, (int)y, c);
        x += xIncrement;
        y += yIncrement;
    }
}

void draw_wireframe_triangle(Color* fb, vec2 p0, vec2 p1, vec2 p2, Color c) {
    draw_line(fb, p0, p1, c);
    draw_line(fb, p1, p2, c);
    draw_line(fb, p2, p0, c);
}


void draw_filled_triangle(Color* fb, vec2 v0, vec2 v1, vec2 v2, Color c) {
    float y0 = v0[Y];
    float y1 = v1[Y];
    float y2 = v2[Y];

    float** p0 = &v0;
    float** p1 = &v1;
    float** p2 = &v2;
    float** t;

#define SWAP_PTRS(a, b) t = a; a = b; b = t
    if(y1 < y0) {SWAP_PTRS(p1, p0);}
    if(y2 < y0) {SWAP_PTRS(p2, p0);}
    if(y2 < y1) {SWAP_PTRS(p2, p1);}
#undef SWAP_PTRS

    float x0 = v0[X]; y0 = v0[Y];
    float x1 = v1[X]; y1 = v1[Y];
    float x2 = v2[X]; y2 = v2[Y];

    pixel_interpolate x02 = start_interpolate(y0, x0, y2, x2);
     
    int y = y0;
    for(
        pixel_interpolate x01 = start_interpolate(y0, x0, y1, x1);
        !done_interpolate(x01);
        step_interpolate(x01)
    ) {
        draw_line(
            fb,
            (vec2){x02.d, y}, 
            (vec2){x01.d, y}, 
            c
        );
        ++y;
        step_interpolate(x02);
    }
    for(
        pixel_interpolate x12 = start_interpolate(y1, x1, y2, x2);
        !done_interpolate(x12);
        step_interpolate(x12)
    ) {
        draw_line(
            fb,
            (vec2){x02.d, y}, 
            (vec2){x12.d, y}, 
            c
        );
        ++y;
        step_interpolate(x02);
    }
}
