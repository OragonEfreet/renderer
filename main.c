#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "raster.h"
#include "linmath.h"

Color framebuffer[CANVAS_WIDTH * CANVAS_HEIGHT];

#define rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0f)

#define COLOR_A 0x212121FF
#define COLOR_B 0x191919FF

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void clear(Color* fb) {
    for(size_t y = 0 ; y < CANVAS_HEIGHT ; ++y) {
        for(size_t x = 0 ; x < CANVAS_WIDTH ; ++x) {
            if (y % 2) {
                *(fb++) = x % 2 ? COLOR_A : COLOR_B;
            } else {
                *(fb++) = x % 2 ? COLOR_B : COLOR_A;
            }
        }
    }
}

void draw_triangles(float t) {

    pixel A = {5*SCALE, 9*SCALE, 0x00};
    pixel B = {-6*SCALE, 3*SCALE, 0xff};
    pixel C = {2*SCALE, -7*SCALE, 0x33};
    pixel D = {9*SCALE, 0*SCALE, 0xFF};

    /* put_pixel(framebuffer, 0, 0, RGB(255, 0, 0)); */

    draw_filled_triangle(framebuffer, A, B, C, COLOR_DARK_CYAN);

    /* put_pixel(framebuffer, A[0], A[1], COLOR_RED); */
    /* put_pixel(framebuffer, B[0], B[1], COLOR_GREEN); */
    /* put_pixel(framebuffer, C[0], C[1], COLOR_BLUE); */

    float dist = 6.f * SCALE;
    float angle_a = 0.f;
    float angle_b = (2.f * M_PI) / 3.f;
    float angle_c = 2.f * angle_b;

    /* t /= 5.f; */


    draw_filled_triangle(
        framebuffer,
        (pixel){ (int)(cosf(angle_a   + t) * dist),   (int)(sinf(angle_a + t) * dist), 0xFF },
        (pixel){ (int)(cosf(angle_b  + t) * dist),  (int)(sinf(angle_b + t) * dist), 0x00 },
        (pixel){ (int)(cosf(angle_c + t) * dist), (int)(sinf(angle_c + t) * dist), 0x33 },
        COLOR_DARK_ORANGE
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

#define VIEWPORT_WIDTH 1.f
#define VIEWPORT_HEIGHT 1.f

void viewport_to_canvas(const vec2 v, pixel p) {
    static const float w_ratio = (float)CANVAS_WIDTH  / VIEWPORT_WIDTH;
    static const float h_ratio = (float)CANVAS_HEIGHT  / VIEWPORT_HEIGHT;

    p[0] = v[0] * w_ratio;
    p[1] = v[1] * h_ratio;
}

void project_vertex(const vec3 v, pixel p) {
    float d = 1.f;
    viewport_to_canvas(
        (vec2) { v[0] * d / v[2], v[1] * d / v[2], }, p
    );
}



void draw_projected_rectangles(float t) {

    {
        // The four "front" vertices
        vec3 vAf = {-2.f, -0.5f, 5.f}; pixel pAf; project_vertex(vAf, pAf);
        vec3 vBf = {-2.f,  0.5f, 5.f}; pixel pBf; project_vertex(vBf, pBf);
        vec3 vCf = {-1.f,  0.5f, 5.f}; pixel pCf; project_vertex(vCf, pCf);
        vec3 vDf = {-1.f, -0.5f, 5.f}; pixel pDf; project_vertex(vDf, pDf);

        // The four "back" vertices
        vec3 vAb = {-2.f, -0.5f, 6.f}; pixel pAb; project_vertex(vAb, pAb);
        vec3 vBb = {-2.f,  0.5f, 6.f}; pixel pBb; project_vertex(vBb, pBb);
        vec3 vCb = {-1.f,  0.5f, 6.f}; pixel pCb; project_vertex(vCb, pCb);
        vec3 vDb = {-1.f, -0.5f, 6.f}; pixel pDb; project_vertex(vDb, pDb);

        // The back face
        draw_line(framebuffer, pAb, pBb, COLOR_RED);
        draw_line(framebuffer, pBb, pCb, COLOR_RED);
        draw_line(framebuffer, pCb, pDb, COLOR_RED);
        draw_line(framebuffer, pDb, pAb, COLOR_RED);

        // The front-to-back edges
        draw_line(framebuffer, pAf, pAb, COLOR_GREEN);
        draw_line(framebuffer, pBf, pBb, COLOR_GREEN);
        draw_line(framebuffer, pCf, pCb, COLOR_GREEN);
        draw_line(framebuffer, pDf, pDb, COLOR_GREEN);

        // The front face
        draw_line(framebuffer, pAf, pBf, COLOR_BLUE);
        draw_line(framebuffer, pBf, pCf, COLOR_BLUE);
        draw_line(framebuffer, pCf, pDf, COLOR_BLUE);
        draw_line(framebuffer, pDf, pAf, COLOR_BLUE);
    }

    {
        t *= 2.f;
        float cx = 1.f;
        float cy = 1.f;
        float cz = 5.f;
        float radius = sinf(0.7f*t);
        float a_45 = M_PI/4.f - t;
        float a_135 = M_PI/2.0 + M_PI/4.f - t;
        float a_225 = 2*M_PI/2.0 + M_PI/4.f - t;
        float a_315 = 3*M_PI/2.0 + M_PI/4.f - t;

        // The four "front" vertices
        vec3 vAf = {cx + cosf(a_45) * radius,  cy + sinf(a_45) * radius, 5.f}; pixel pAf; project_vertex(vAf, pAf);
        vec3 vBf = {cx + cosf(a_135) * radius, cy + sinf(a_135) * radius, 5.f}; pixel pBf; project_vertex(vBf, pBf);
        vec3 vCf = {cx + cosf(a_225) * radius, cy + sinf(a_225) * radius, 5.f}; pixel pCf; project_vertex(vCf, pCf);
        vec3 vDf = {cx + cosf(a_315) * radius, cy + sinf(a_315) * radius, 5.f}; pixel pDf; project_vertex(vDf, pDf);
        
        // The four "back" vertices
        vec3 vAb = {cx + cosf(a_45) * radius,  cy + sinf(a_45) * radius, 6.f}; pixel pAb; project_vertex(vAb, pAb);
        vec3 vBb = {cx + cosf(a_135) * radius,  cy + sinf(a_135) * radius, 6.f}; pixel pBb; project_vertex(vBb, pBb);
        vec3 vCb = {cx + cosf(a_225) * radius,  cy + sinf(a_225) * radius, 6.f}; pixel pCb; project_vertex(vCb, pCb);
        vec3 vDb = {cx + cosf(a_315) * radius,  cy + sinf(a_315) * radius, 6.f}; pixel pDb; project_vertex(vDb, pDb);


        // The back face
        draw_line(framebuffer, pAb, pBb, COLOR_DARK_BLUE);
        draw_line(framebuffer, pBb, pCb, COLOR_DARK_BLUE);
        draw_line(framebuffer, pCb, pDb, COLOR_DARK_BLUE);
        draw_line(framebuffer, pDb, pAb, COLOR_DARK_BLUE);

        // The front-to-back edges
        draw_line(framebuffer, pAf, pAb, COLOR_GREEN);
        draw_line(framebuffer, pBf, pBb, COLOR_GREEN);
        draw_line(framebuffer, pCf, pCb, COLOR_GREEN);
        draw_line(framebuffer, pDf, pDb, COLOR_GREEN);

        // The front face
        draw_line(framebuffer, pAf, pBf, COLOR_RED);
        draw_line(framebuffer, pBf, pCf, COLOR_RED);
        draw_line(framebuffer, pCf, pDf, COLOR_RED);
        draw_line(framebuffer, pDf, pAf, COLOR_RED);

    }



}

void draw(float t) {
    clear(framebuffer);
    draw_triangles(t);
    draw_projected_rectangles(t);
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
