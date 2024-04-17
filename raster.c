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

/* typedef struct { */
/*     float a; */
/*     float d; */
/* } interp; */

/* static inline interp interp_begin(int i0, float d0, int i1, float d1) { */
/*     return (interp) { */
/*         .a = (d1 - d0) / ((float)i1 - (float)i0), */
/*         .d = d0 */
/*ecl_interp     }; */
/* } */

/* static inline float interp_step(interp* i) { */
/*     return i->d += i->a; */
/* } */

#define interp_a(i0,d0,i1,d1) ((float)((d1)-(d0)))/((float)((i1)-(i0)))
#define interp_begin(F, i0,d0,i1,d1) float F ## _a = interp_a(i0,d0,i1,d1), F ## _d = d0
#define interp_value(F) (F ## _d)
#define interp_step(F) F ## _d += F ## _a

static void set_pixel(Color* fb, int x, int y, Color c) {
    /* Color* ofst = fb+XY(x, y); */
    /* if(*ofst != 0x00000000) */
    /*     c = COLOR_RED; */
    put_pixel(fb, x,y,c);
}


static bool pa = true;


#define SWAP_INT(a, b) a=a^b;b=a^b;a=a^b
#define SWAP_PXL(a, b) SWAP_INT(a[X], b[X]);SWAP_INT(a[Y], b[Y]);SWAP_INT(a[H], b[H])
    void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {


        // Sorts p0 < p1 < p2
        if(p1[Y] < p0[Y]) {SWAP_PXL(p1, p0);}
        if(p2[Y] < p0[Y]) {SWAP_PXL(p2, p0);}
        if(p2[Y] < p1[Y]) {SWAP_PXL(p2, p1);}
        /* printf("p0: %d,%d / 0x%x\n", p0[X],p0[Y],p0[H]); */
        /* printf("p1: %d,%d / 0x%x\n", p1[X],p1[Y],p1[H]); */
        /* printf("p2: %d,%d / 0x%x\n", p2[X],p2[Y],p2[H]); */


        interp_begin(x02, p0[Y], p0[X], p2[Y], p2[X]);
        interp_begin(h02, p0[Y], p0[H], p2[Y], p2[H]);

        // First triangle half
        interp_begin(x01, p0[Y], p0[X], p1[Y], p1[X]);
        interp_begin(h01, p0[Y], p0[H], p1[Y], p1[H]);
        for(int y = p0[Y] ; y < p1[Y] ; ++y) {
            int left_x = interp_value(x01); 
            int right_x = interp_value(x02); 

            float left_h = interp_value(h01); 
            float right_h = interp_value(h02); 
            if (pa)printf("\nY %d: ", y);
            /* printf("L:%f R:%f |", left_h, right_h); */

            if(right_x < left_x) {
                SWAP_INT(left_x, right_x);
                float tmp = left_h; left_h = right_h; right_h = tmp;
            }

            interp_begin(h, left_x, left_h, right_x, right_h);
            for(int x = left_x ; x <= right_x ; ++x) {
                float h_factor = interp_value(h) / 255.f;
                if (pa)printf("%d: %f |", x, h_factor);
                /* Color frag = c; */
                Color frag = RGB(
                    (uint8_t)((float)C_RED(c) * h_factor) ,
                    (uint8_t)((float)C_GREEN(c) * h_factor) ,
                    (uint8_t)((float)C_BLUE(c) * h_factor) 
                );
                set_pixel(fb, x, y, frag);

                interp_step(h);
            }

            interp_step(x01);
            interp_step(x02);
            interp_step(h01);
            interp_step(h02);
        }



        // Second triangle half
        interp_begin(x12, p1[Y], p1[X], p2[Y], p2[X]);
        interp_begin(h12, p1[Y], p1[H], p2[Y], p2[H]);
        for(int y = p1[Y] ; y <= p2[Y] ; ++y) {
            int left_x = interp_value(x12); 
            int right_x = interp_value(x02); 

            float left_h = interp_value(h12); 
            float right_h = interp_value(h02); 

            if(right_x < left_x) {
                SWAP_INT(left_x, right_x);
                float tmp = left_h; left_h = right_h; right_h = tmp;
            }

            interp_begin(h, left_x, left_h, right_x, right_h);
            for(int x = left_x ; x <= right_x ; ++x) {
                float h_factor = interp_value(h) / 255.f;
                Color frag = RGB(
                    (uint8_t)((float)C_RED(c) * h_factor) ,
                    (uint8_t)((float)C_GREEN(c) * h_factor) ,
                    (uint8_t)((float)C_BLUE(c) * h_factor) 
                );
                set_pixel(fb, x, y, frag);
                interp_step(h);
            }

            interp_step(x12);
            interp_step(x02);
            interp_step(h12);
            interp_step(h02);
        }

        pa=false;
    }
#undef SWAP_PXL
#undef SWAP_INT

    /* void draw_filled_triangle_old(Color* fb, pixel p0, pixel p1, pixel p2, Color c) { */
    /* // Sorts p0 < p1 < p2 */
    /* if(p1[Y] < p0[Y]) {SWAP_PXL(p1, p0);} */
    /* if(p2[Y] < p0[Y]) {SWAP_PXL(p2, p0);} */
    /* if(p2[Y] < p1[Y]) {SWAP_PXL(p2, p1);} */

    /* // Make interpolation variables for the triangle edges */
    /* interp h01 = interp_begin(p0[Y], ((float)p0[H]) / 255.f, p1[Y], ((float)p1[H]) / 255.f); */
    /* interp x01 = interp_begin(p0[Y], p0[X], p1[Y], p1[X]); */
    /* interp x12 = interp_begin(p1[Y], p1[X], p2[Y], p2[X]); */
    /* interp x02 = interp_begin(p0[Y], p0[X], p2[Y], p2[X]); */
    /* interp h12 = interp_begin(p1[Y], ((float)p1[H]) / 255.f, p2[Y], ((float)p2[H]) / 255.f); */
    /* interp h02 = interp_begin(p0[Y], ((float)p0[H]) / 255.f, p2[Y], ((float)p2[H]) / 255.f); */

    /* for(int y = p0[Y] ; y < p1[Y] ; ++y) { */
    /*     int x_left = x01.d; */
    /*     int x_right = x02.d; */

    /*     float h_left = h01.d; */
    /*     float h_right = h02.d; */

    /*     if (x_right < x_left) { */
    /*         SWAP_INT(x_right, x_left); */
    /*         float tmp = h_left; */
    /*         h_left = h_right; */
    /*         h_right = tmp; */
    /*     } */

    /*     for(int x = x_left ; x <= x_right; ++x) { */
    /*         interp h = interp_begin(x_left, h_left, x_right, h_right); */
    /*         Color frag_color = RGB( */
    /*             (C_RED(c) * h.d), */
    /*             (C_GREEN(c) * h.d), */
    /*             (C_BLUE(c) * h.d) */
    /*         ); */

    /*         printf("%f\n", h.d); */
    /*         interp_step(&h); */
    /*         put_pixel(fb, x, y, frag_color); */
    /*     } */

    /*     interp_step(&x01); */
    /*     interp_step(&x02); */
    /*     interp_step(&h01); */
    /* } */

    /* for(int y = p1[Y] ; y < p2[Y] ; ++y) { */
    /*     int x_left = x12.d; */
    /*     int x_right = x02.d; */

    /*     float h_left = h12.d; */
    /*     float h_right = h02.d; */

    /*     if (x_right < x_left) { */
    /*         SWAP_INT(x_right, x_left); */
    /*         float tmp = h_left; */
    /*         h_left = h_right; */
    /*         h_right = tmp; */
    /*     } */

    /*     for(int x = x_left ; x <= x_right; ++x) { */
    /*         interp h = interp_begin(x_left, h_left, x_right, h_right); */
    /*         Color frag_color = RGB( */
    /*             (C_RED(c) * h.d), */
    /*             (C_GREEN(c) * h.d), */
    /*             (C_BLUE(c) * h.d) */
    /*         ); */

    /*         printf("%f\n", h.d); */
    /*         interp_step(&h); */
    /*         put_pixel(fb, x, y, frag_color); */
    /*     } */

    /*     interp_step(&x12); */
    /*     interp_step(&x02); */
    /*     interp_step(&h12); */
    /* } */


    /*     float h_left = h12.d; */
    /*     float h_right = h12.d; */

    /*     if (x_right < x_left) { */
    /*         SWAP_INT(x_right, x_left); */
    /*         float tmp = h_left; */
    /*         h_left = h_right; */
    /*         h_right = tmp; */
    /*     } */

    /*     for(int x = x_left ; x <= x_right; ++x) { */
    /*         interp h = interp_begin(x_left, h_left, x_right, h_right); */
    /*         Color frag_color = RGB( */
    /*             (C_RED(c) * h.d), */
    /*             (C_GREEN(c) * h.d), */
    /*             (C_BLUE(c) * h.d) */
    /*         ); */

    /*         printf("%f\n", h.d); */
    /*         interp_step(&h); */
    /*         put_pixel(fb, x, y, frag_color); */
    /*     } */

    /*     interp_step(&x12); */
    /*     interp_step(&x02); */
    /*     interp_step(&h12); */
    /* } */

    /* for(int y = p1[Y] ; y <= p2[Y] ; ++y) { */
    /*     int x_left = x12.d; */
    /*     int x_right = x02.d; */

    /*     if (x_right < x_left) {SWAP_INT(x_right, x_left);} */

    /*     for(int x = x_left ; x <= x_right; ++x) { */
    /*         put_pixel(fb, x, y, c); */
    /*     } */

    /*     interp_step(&x12); */
    /*     interp_step(&x02); */
    /* } */
/* } */


