#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <SDL2/SDL.h>
#include <bitset>
#include "LTextures.h"

#ifndef LEditor_H
#define LEditor_H

#define MAPSIZE 64
#define ARRAYSIZE 4096

using namespace std;

class LEditor {

private:
    array<u_int8_t, ARRAYSIZE> roomTemplate;
    LTextures textures;
public:
    LEditor(LTextures textures);
    void loadTemplate(string);
    void saveRoom();
    void debugFile();
    void updateSegment(int x, int y, int value);
    void drawMap(SDL_Renderer*);
    void incrememntSegment(int x, int y);
};

#endif
