#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <SDL2/SDL.h>
#include "src/LEditor.h"
#include "src/LTextures.h"

#define GRIDSIZE 8
#define GRIDX 64
#define WIDTH 640
#define HEIGHT 512

using namespace std;

array<u_int8_t, 64*64> roomTemplate;

SDL_Renderer* renderer;
SDL_Window* window;


LTextures textures;

bool finished = false;

void drawGrid() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= GRIDX; i++) {
        SDL_RenderDrawLine(renderer, i * GRIDSIZE, 0, i * GRIDSIZE, HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * GRIDSIZE, HEIGHT, i * GRIDSIZE);
    }
}

int main(int argv, char** argc) {

    cout << "Started" << endl;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC, &window, &renderer);

    textures = LTextures();
    textures.generateTextures();

    LEditor editor = LEditor(textures);
    editor.loadTemplate("./bin/file.bin");

    while (!finished) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                finished = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    finished = true;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    editor.saveRoom();
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x /= GRIDSIZE;
                y /= GRIDSIZE;
                editor.incrememntSegment(x, y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawGrid();
        editor.drawMap(renderer);


        SDL_RenderPresent(renderer);
    }

    return 0;
}