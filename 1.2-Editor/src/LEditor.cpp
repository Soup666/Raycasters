#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "LEditor.h"

using namespace std;

LEditor::LEditor() {
}

void LEditor::loadTemplate(string path = "") {

    ifstream fin;

    if (path == "")
        fin.open("backup.bin", ios::binary);
    else
        fin.open(path, ios::binary);

    vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

    int count = 0;
    for (char index : buffer) {
        roomTemplate[count++] = (int)index;
    }

    fin.close();

}

void LEditor::saveRoom(array<u_int8_t, ARRAYSIZE> templateA) {

    ofstream fout;

    fout.open("file.bin", ios::binary | ios::out);

    fout.write((char*) &templateA, sizeof(u_int8_t) * 4096);

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

