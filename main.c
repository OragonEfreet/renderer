#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef uint32_t Color;

#define RGB(r, g, b) (Color)((((uint8_t)r & 0xFF) << 24) | (((uint8_t)g & 0xFF) << 16) | (((uint8_t)b & 0xFF) << 8) | (uint8_t)0xFF)
#define RED_C(c) (uint8_t)((c >> 24) & 0xFF)
#define GREEN_C(c) (uint8_t)((c >> 16) & 0xFF)
#define BLUE_C(c) (uint8_t)((c >> 8) & 0xFF)
#define ALPHA_C(c) (uint8_t)(c & 0xFF)

SDL_Renderer* renderer = 0;

void put_pixel(size_t x, size_t y, Color c) {
    SDL_SetRenderDrawColor(renderer, RED_C(c), GREEN_C(c), BLUE_C(c), ALPHA_C(c));
    SDL_RenderDrawPoint(renderer, (CANVAS_WIDTH / 2) + x, (CANVAS_HEIGHT / 2) - y);
}

void draw() {
    Color red = RGB(255, 0, 0);
    Color green = RGB(0, 255, 0);
    Color blue = RGB(0, 0, 255);
    put_pixel(100, 100, red);
    put_pixel(200, 100, green);
    put_pixel(300, 100, blue);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL2 Boilerplate", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Main loop flag
    bool quit = false;
    SDL_Event e;

    /* // Clear the screen */
    /* SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); */
    /* SDL_RenderClear(renderer); */
    draw();


    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_KEYUP) {
                quit = true;
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
