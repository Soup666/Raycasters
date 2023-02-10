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

#endif