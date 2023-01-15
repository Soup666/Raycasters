#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

u_int8_t roomTemplate[64*64];

void loadTemplate() {

    ifstream fin;

    fin.open("backup.bin", ios::binary);
    vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

    int count = 0;
    for (char index : buffer) {
        roomTemplate[count++] = (int)index;
    }

    fin.close();

}

void saveRoom() {

    ofstream fout;

    fout.open("file.bin", ios::binary | ios::out);

    fout.write((char*) &roomTemplate, sizeof(roomTemplate));

    fout.close();

}

void debugFile() {

    ifstream fin;

    fin.open("file.bin", ios::binary);
    vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

    for (auto index : buffer) {
        cout << (int)index << endl;
    }

    fin.close();

}

void debugRoom() {
    for (auto index : roomTemplate) cout << (int)index << endl;
}

int main(int argv, char** argc) {

    loadTemplate();
    debugRoom();

    // Read
    
    return 0;
}