#include <array>

#ifndef LEditor_H
#define LEditor_H

#define MAPSIZE 64
#define ARRAYSIZE 4096

using namespace std;

class LEditor {

private:
    array<u_int8_t, ARRAYSIZE> roomTemplate;
public:
    LEditor();
    void loadTemplate(string);
    void saveRoom(array<u_int8_t, ARRAYSIZE>);
    void debugFile();
    void updateSegment(int x, int y, int value);
};

#endif
