#ifndef _Generator_H
#define _Generator_H

#include <Arduino.h>
#include "Globals.h"

#define pi 3.14159265358979323846
#define Latitude -25.7637802
#define Longitude 28.2457732
#define Radius 10000

class Generator
{
public:
    deviceStatus_t GetRandomStatus();

private:
    deviceStatus_t deviceStatus;
    void SetLocation(double x0, double y0, int radius);
    void SetTemperature();
    void SetAltitude();
    double toRadians(double degree);
    double doubleRand();
    int randomInt();
    double getRandomDoubleInRange(double fMin, double fMax);
};

#endif