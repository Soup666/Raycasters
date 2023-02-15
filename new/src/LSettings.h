/*
    Outlines the Settings Class and global functions used.
*/

#ifndef LSettings_h
#define LSettings_h

#include <math.h>
#include <map>
#include <string>
#include <iostream>

class LSettings
{
public:
    LSettings();
    static int getISetting(std::string name);
    static double getDSetting(std::string name);
private:
    static std::map<std::string, int> iSettings;
    static std::map<std::string, double> dSettings;
    static void addISetting(std::string name, int value);
    static void addDSetting(std::string name, double value);
};

extern void fixAngle(int &a);
extern void fixAngle(double &a);
extern double degToRad(int deg);
extern double degToRad(double deg);

extern double FOV;
extern int WIDTH;
extern int HEIGHT;
extern int DEBUG;
extern int PWIDTH;
extern int PHEIGHT;
extern int DOF;
extern int MAPR;
extern int MAPC;
extern int mapMultiplier;
extern int WINDOW_PADDING_WIDTH;
extern int WINDOW_PADDING_HEIGHT;

#endif