#include "LSettings.h"

std::map<std::string, int> LSettings::iSettings = {
    {"WIDTH", 720},
    {"HEIGHT", 640},
    {"DEBUG", 0},
    {"PWIDTH", 500},
    {"PHEIGHT", 500},
    {"DOF", 64},
    {"MAPR", 64},
    {"MAPC", 64},
    {"texWidth", 64},
    {"texHeight", 64},
    {"mapMultiplier", 10},
    {"WINDOW_PADDING_WIDTH", 110},
    {"WINDOW_PADDING_HEIGHT", 60}
};

std::map<std::string, double> LSettings::dSettings = {
    {"FOV", 60.0},
    {"bobbing", 0.0}
};

double FOV = 60.0;

int WIDTH = 720;
int HEIGHT = 640;
int DEBUG = 0;
int PWIDTH = 500;
int PHEIGHT = 500;
int DOF = 64;
int MAPR = 64;
int MAPC = 64;
int mapMultiplier = 10;
int WINDOW_PADDING_WIDTH = 110;
int WINDOW_PADDING_HEIGHT = 60;

LSettings::LSettings() {

};

int LSettings::getISetting(std::string name) {
    return iSettings[name];
}

double LSettings::getDSetting(std::string name) {
    return dSettings[name];
}

double degToRad(int deg)
{
    return deg * (M_PI / 180);
}

double degToRad(double deg)
{
    return deg * (M_PI / 180.0);
}


void fixAngle(int &a) {
    if (a < 0) a += 360;
    if (a > 360) a -= 360;
}

void fixAngle(double &a) {
    if (a < 0) a += 360.0;
    if (a > 360.0) a -= 360.0;
}