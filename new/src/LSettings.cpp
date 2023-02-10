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
    {"WINDOW_PADDING_WIDTH", (720 - 500) / 2},
    {"WINDOW_PADDING_HEIGHT", (640 - 500) / 2}
};

std::map<std::string, double> LSettings::dSettings = {
    {"FOV", 60.0},
    {"bobbing", 0.0}
};

LSettings::LSettings() {

    // LSettings::addDSetting("FOV", 60.0);
    // // dSettings["bobbing"] = (sin(degToRad(SDL_GetTicks64())) * pSpeed * W * 0.5);
    // LSettings::addDSetting("bobbing",  0.0);

    // LSettings::addISetting("WIDTH", 720);
    // LSettings::addISetting("HEIGHT", 640);
    // LSettings::addISetting("DEBUG", 0);
    // LSettings::addISetting("PWIDTH", 500);
    // LSettings::addISetting("PHEIGHT", 500);
    // LSettings::addISetting("DOF", 64);
    // LSettings::addISetting("MAPR", 64);
    // LSettings::addISetting("MAPC", 64);
    // LSettings::addISetting("texWidth", 64);
    // LSettings::addISetting("texHeight", 64);
    // LSettings::addISetting("mapMultiplier", 10);
    // LSettings::addISetting("WINDOW_PADDING_WIDTH", (720 - 500) / 2);
    // LSettings::addISetting("WINDOW_PADDING_HEIGHT", (640 - 500) / 2);

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


void fixAngle(int &a) {
    if (a < 0) a += 360;
    if (a > 360) a -= 360;
}

void fixAngle(double &a) {
    if (a < 0) a += 360.0;
    if (a > 360.0) a -= 360.0;
}