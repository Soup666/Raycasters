
#include <vector>
#include <cmath>
#include <array>
#include <iostream>

#ifndef LTextures_H
#define LTextures_H

#define texWidth 64
#define texHeight 64

using namespace std;

class LTextures {
    private:
        array<array<u_int32_t, 4096>, 8> texture;
    public:
        void generateTextures();
        array<u_int8_t, 3> textureToWall(int mapValue, int x, int y);
};

#endif