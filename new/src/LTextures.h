/*
    Outlines the Textures class.
*/

#include <vector>
#include <cmath>
#include <array>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef LTextures_H
#define LTextures_H

#define texWidth 64
#define texHeight 64

using namespace std;

class LTextures {
    private:
        SDL_Surface* texture[8];
        SDL_Surface* defaultTex;
        Uint32 pf;
        SDL_Renderer *ren;
        Uint8 *CopySurfacePixels(SDL_Surface *surface, int *width, int *height, int *pitch);

    public:
        LTextures();
        LTextures(Uint32 pixelFormat, SDL_Renderer *renderer);
        // void generateTextures();
        SDL_Color textureToWall(int mapValue, int x, int y);
        int getTextureCount();

};

#endif