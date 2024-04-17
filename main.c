#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "raster.h"

Color framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

#define rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0f)

#define COLOR_A 0x212121FF
#define COLOR_B 0x191919FF

void clear(Color* fb) {
    /* for(size_t y = 0 ; y < CANVAS_HEIGHT ; ++y) { */
    /*     for(size_t x = 0 ; x < CANVAS_WIDTH ; ++x) { */
    /*         if (y % 2) { */
    /*             *(fb++) = x % 2 ? COLOR_A : COLOR_B; */
    /*         } else { */
    /*             *(fb++) = x % 2 ? COLOR_B : COLOR_A; */
    /*         } */
    /*     } */
    /* } */
    memset(fb, 0x00, sizeof(Color) * CANVAS_WIDTH * CANVAS_HEIGHT);
}

void draw(float t) {
    clear(framebuffer);

    pixel A = {5*SCALE, 9*SCALE, 0x00};
    pixel B = {-6*SCALE, 3*SCALE, 0xff};
    pixel C = {2*SCALE, -7*SCALE, 0x22};
    pixel D = {9*SCALE, 0*SCALE, 0xFF};

    /* put_pixel(framebuffer, 0, 0, RGB(255, 0, 0)); */

    /* draw_filled_triangle(framebuffer, A, B, C, RGB(0, 255, 0)); */

     put_pixel(framebuffer, A[0], A[1], COLOR_RED);
     put_pixel(framebuffer, B[0], B[1], COLOR_GREEN);
     put_pixel(framebuffer, C[0], C[1], COLOR_BLUE);

    float dist = 6.f * SCALE;
    float angle_a = 0.f;
    float angle_b = (2.f * M_PI) / 3.f;
    float angle_c = 2.f * angle_b;

    /* t /= 5.f; */


    draw_filled_triangle(
        framebuffer,
        (pixel){ (int)(cosf(angle_a   + t) * dist),   (int)(sinf(angle_a + t) * dist), 0xFF },
        (pixel){ (int)(cosf(angle_b  + t) * dist),  (int)(sinf(angle_b + t) * dist), 0x00 },
        (pixel){ (int)(cosf(angle_c + t) * dist), (int)(sinf(angle_c + t) * dist), 0x00 },
        COLOR_ORANGE
    );

    /* draw_wireframe_triangle( */
    /*     framebuffer, */
    /*     v0, v1, v2, */
    /*     COLOR_SPRING_GREEN */
    /* ); */


     put_pixel(framebuffer, 0, 1, 0x004900FF);
     put_pixel(framebuffer, 1, 0, 0x000049FF);
     put_pixel(framebuffer, 0, 0, 0x494949FF);




}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    assert(window != 0);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    assert(renderer != 0);

    // Create the rendering texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);

    // Main loop flag
    bool quit = false;
    SDL_Event e;


    // Main loop
    float t = 0.f;
    const float speed = .5f;
    Uint64 ticks = SDL_GetTicks64();
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_KEYUP) {
                quit = true;
            }
        }
        Uint64 current_ticks = SDL_GetTicks64();
        t += ((float)(current_ticks - ticks) / 1000.f) * speed;
        ticks = current_ticks;

        draw(t);
        SDL_UpdateTexture(texture, 0, framebuffer, CANVAS_WIDTH * sizeof (uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, 0, 0);
        SDL_RenderPresent(renderer);
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
