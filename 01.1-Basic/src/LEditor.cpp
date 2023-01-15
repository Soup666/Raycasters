#include <iostream>
#include <fstream>
#include <vector>

#ifndef LEditor_H

#define LEditor_H

using namespace std;


class LEditor {

    public:
        u_int8_t roomTemplate[64*64];

        void loadTemplate(u_int8_t templateA[64*64]) {

            ifstream fin;

            fin.open("backup.bin", ios::binary);
            vector<unsigned char> buffer(istreambuf_iterator<char>(fin), {});

            int count = 0;
            for (char index : buffer) {
                cout << (int)index << endl;
                templateA[count++] = (int)index;
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

};


#endif