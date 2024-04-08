#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "linmath.h"
#include <SDL2/SDL.h>

#define CANVAS_WIDTH 640
#define CANVAS_HEIGHT 480

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

typedef uint32_t Color;

#define RGB(r, g, b) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)0xFF)

Color framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

#define X 0
#define Y 1
#define S_X(x) ((CANVAS_WIDTH / 2) + x)
#define S_Y(y) ((CANVAS_HEIGHT / 2) - y)
#define XY(x, y) (S_Y(y) * CANVAS_WIDTH + S_X(x))
#define put_pixel(x, y, c) framebuffer[XY(x, y)] = c;

/* void put_pixel(int x, int y, Color c) { */
/*     int offset = XY(x, y); */
/*     if(offset >= 0 && offset < (CANVAS_WIDTH * CANVAS_HEIGHT)) { */
/*         framebuffer[offset] = c; */
/*     } else { */
/*         printf("Out of bound (%d, %d)\n", x, y); */
/*     } */
/* } */

////////////////////////////////////////////////////////////////////////////////
/// Drawing

/// Write a single pixel on the canvas


#ifdef DONT_TRUST_CHATGPT
void draw_line(vec2 p0, vec2 p1, Color c) {
    float x0 = p0[X]; float y0 = p0[Y];
    float x1 = p1[X]; float y1 = p1[Y];

    const int dx = x1 - x0;
    const int dy = y1 - y0;

    if(abs(dx) > abs(dy)) {
        if(x0 > x1) { // Swap
            const float temp = x0;
            x0 = x1;
            x1 = temp;
        }

        const float a = (y1 - y0) / (x1 - x0);
        float y = y0;

        for(int x = x0 ; x <= x1 ; ++x) {
            put_pixel(x, (int)y, c);
            y += a;
        }
    } else {
        if(y0 > y1) { // Swap
            const float temp = y0;
            y0 = y1;
            y1 = temp;
        }

        const float a = (x1 - x0) / (y1 - y0);
        float x = x0;

        for(int y = y0 ; y <= y1 ; ++y) {
            put_pixel((int)x, y, c);
            x += a;
        }
    }
}
#else
void draw_line(vec2 p0, vec2 p1, Color c) {
    float dx = p1[X] - p0[X];
    float dy = p1[Y] - p0[Y];
    int steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    float x = p0[X];
    float y = p0[Y];

    for(int i = 0; i <= steps; ++i) {
        put_pixel((int)x, (int)y, c);
        x += xIncrement;
        y += yIncrement;
    }
}
#endif

/// Main drawing function
void draw() {

    draw_line(
        (vec2){-200.0f, -100.0f}, 
        (vec2){240.f, 140.0f}, 
        RGB(255, 0, 0)
    );

    draw_line(
        (vec2){-50.0f, -200.0f}, 
        (vec2){60.0f, 240.0f}, 
        RGB(0, 255, 0)
    );

    /* Color red = RGB(255, 0, 0); */
    /* Color green = RGB(0, 255, 0); */
    /* Color blue = RGB(0, 0, 255); */
    /* put_pixel(100, 100, red); */
    /* put_pixel(200, 100, green); */
    /* put_pixel(300, 100, blue); */

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

    draw();
    SDL_UpdateTexture(texture, 0, framebuffer, CANVAS_WIDTH * sizeof (uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);

    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_KEYUP) {
                quit = true;
            }
        }
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
