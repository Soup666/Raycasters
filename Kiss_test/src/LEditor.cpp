
#include "LEditor.h"

using namespace std;

LEditor::LEditor(LTextures textures) {
    textures.generateTextures();
    this->textures = textures;
}

void LEditor::loadTemplate(string path = "") {

    ifstream fin;

    if (path == "")
        fin.open("./bin/backup.bin", ios::binary);
    else
        fin.open(path, ios::binary);

    vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

    int count = 0;
    cout << sizeof(buffer) << endl;
    for (char index : buffer) {
        roomTemplate[count++] = (int)index;
    }

    fin.close();

}

array<u_int8_t, ARRAYSIZE> LEditor::getMap() {
    return roomTemplate;
}

void LEditor::saveRoom() {

    ofstream fout;

    fout.open("file.bin", ios::binary | ios::out);

    fout.write((char*) &roomTemplate, sizeof(u_int8_t) * 4096);

    fout.close();

}

void LEditor::debugFile() {

    ifstream fin;

    fin.open("file.bin", ios::binary);
    vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

    for (auto index : buffer) {
        cout << (int)index << endl;
    }

    fin.close();

}

void LEditor::updateSegment(int x, int y, int value) {
    roomTemplate[x + y * MAPSIZE] = value;
}

void LEditor::incrememntSegment(int x, int y) {
    roomTemplate[x + y * MAPSIZE] = roomTemplate[x + y * MAPSIZE] + 1;
    if (roomTemplate[x + y * MAPSIZE] > 7)
        roomTemplate[x + y * MAPSIZE] = 0;
}

void LEditor::drawMap(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < MAPSIZE; i++) {
        for (int j = 0; j < MAPSIZE; j++) {

            int mapValue = roomTemplate[i + j * MAPSIZE];

            if (mapValue != 0) {

                for (int pixelX = 0; pixelX < 8; pixelX++) {
                    for (int pixelY = 0; pixelY < 8; pixelY++) {
                        
                        array<u_int8_t, 3> pixelValue = textures.textureToWall(mapValue, pixelX * 8, pixelY * 8); // * 8 cause 8x8 = 64 (its 1/8th of the texture size)
                        SDL_SetRenderDrawColor(renderer, pixelValue[0], pixelValue[1], pixelValue[2], 255);
                        SDL_RenderDrawPoint(renderer, i * 8 + pixelX, j * 8 + pixelY);

                    }
                }



                // SDL_Rect rect = { i * 8, j * 8, 8, 8 };
                // SDL_RenderFillRect(renderer, &rect);
            }

        }
    }
}
