#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "raster.h"

Color framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

#define rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0f)

void draw(float t) {
    memset(framebuffer, 0, sizeof(Color) * CANVAS_WIDTH * CANVAS_HEIGHT);


    /* put_pixel(100, 100, RGB(255, 0, 0)); */

    /* draw_line( */
    /*     (vec2){-200.0f, -100.0f}, */ 
    /*     (vec2){240.f, 140.0f}, */ 
    /*     RGB(255, 0, 0) */
    /* ); */

    /* draw_line( */
    /*     (vec2){-50.0f, -200.0f}, */ 
    /*     (vec2){60.0f, 240.0f}, */ 
    /*     RGB(0, 255, 0) */
    /* ); */


    /* draw_wireframe_triangle( */
    /*     framebuffer, */
    /*     (vec2){-200.f, -250.f}, */ 
    /*     (vec2){200.f, 50.f}, */ 
    /*     (vec2){20.f, 250.f}, */ 
    /*     RGB(0, 255, 0) */
    /* ); */
     const float a0 = rad(0.f) + t; 
     const float cos_a0 = cosf(a0); 
     const float sin_a0 = sinf(a0); 

     const float a1 = rad(45.f) + t; 
     const float cos_a1 = cosf(a1); 
     const float sin_a1 = sinf(a1); 

     const float a2 = rad(180.f) + t; 
     const float cos_a2 = cosf(a2); 
     const float sin_a2 = sinf(a2); 

     const float r0 = 100.f; 
     const float r1 = 200.f; 
     const float r2 = 150.f; 
    
     float v0x = cos_a0 * r0;
     float v0y = sin_a0 * r0;
     vec2 v0 = { v0x, v0y };

     float v1x = cos_a1 * r1;
     float v1y = sin_a1 * r1;
     vec2 v1 = { v1x, v1y };

     float v2x = cos_a2 * r2;
     float v2y = sin_a2 * r2;
     vec2 v2 = { v2x, v2y };


    draw_filled_triangle(
        framebuffer,
        v0, v1, v2,
        RGB(0, 0, 255)
    );

    draw_wireframe_triangle(
        framebuffer,
        v0, v1, v2,
        RGB(255, 0, 0)
    );

     /* put_pixel(framebuffer, v0x, v0y, COLOR_WHITE); */
     /* put_pixel(framebuffer, v1x, v1y, COLOR_WHITE); */
     /* put_pixel(framebuffer, v2x, v2y, COLOR_WHITE); */

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
    const float speed = 2.5f;
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
