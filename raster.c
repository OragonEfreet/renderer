#include <stdbool.h>
#include <stdlib.h>

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

#define interp_a(i0,d0,i1,d1) ((float)((d1)-(d0)))/((float)((i1)-(i0)))
#define interp_begin(F, i0,d0,i1,d1) float F ## _a = interp_a(i0,d0,i1,d1), F ## _d = d0
#define interp_value(F) (F ## _d)
#define interp_step(F) F ## _d += F ## _a

#define SWAP_INT(a, b) a=a^b;b=a^b;a=a^b
#define SWAP_PXL(a, b) SWAP_INT(a[X], b[X]);SWAP_INT(a[Y], b[Y]);SWAP_INT(a[H], b[H])
void draw_filled_triangle(Color* fb, pixel p0, pixel p1, pixel p2, Color c) {

        // Sorts p0 < p1 < p2
        if(p1[Y] < p0[Y]) {SWAP_PXL(p1, p0);}
        if(p2[Y] < p0[Y]) {SWAP_PXL(p2, p0);}
        if(p2[Y] < p1[Y]) {SWAP_PXL(p2, p1);}

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
            /* printf("L:%f R:%f |", left_h, right_h); */

            if(right_x < left_x) {
                SWAP_INT(left_x, right_x);
                float tmp = left_h; left_h = right_h; right_h = tmp;
            }

            interp_begin(h, left_x, left_h, right_x, right_h);
            for(int x = left_x ; x <= right_x ; ++x) {
                float h_factor = interp_value(h) / 255.f;
                /* Color frag = c; */
                put_pixel(fb, x, y, RGB(
                    (uint8_t)((float)C_RED(c) * h_factor) ,
                    (uint8_t)((float)C_GREEN(c) * h_factor) ,
                    (uint8_t)((float)C_BLUE(c) * h_factor) 
                ));

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
                put_pixel(fb, x, y, RGB(
                    (uint8_t)((float)C_RED(c) * h_factor) ,
                    (uint8_t)((float)C_GREEN(c) * h_factor) ,
                    (uint8_t)((float)C_BLUE(c) * h_factor) 
                ));
                interp_step(h);
            }

            interp_step(x12);
            interp_step(x02);
            interp_step(h12);
            interp_step(h02);
        }
    }
#undef SWAP_PXL
#undef SWAP_INT

