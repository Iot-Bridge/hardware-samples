#include "Generator.h"

deviceStatus_t Generator::GetRandomStatus()
{
    srand(esp_random());
    SetLocation(Longitude, Latitude, Radius);
    SetTemperature();
    SetAltitude();
    SetSatellites();

    return deviceStatus;
}

void Generator::SetLocation(double x0, double y0, int radius)
{
    double n = 111000;
    // Convert radius from meters to degrees
    double radiusInDegrees = radius / n;

    double u = doubleRand();
    double v = doubleRand();

    double w = radiusInDegrees * sqrt(u);
    double t = 2 * pi * v;
    double x = w * cos(t);
    double y = w * sin(t);

    // Adjust the x-coordinate for the shrinking of the east-west distances
    double new_x = x / cos(toRadians(y0));

    double foundLongitude = new_x + x0;
    double foundLatitude = y + y0;

    deviceStatus.longitude = (int32_t)(foundLongitude * 1e6);
    deviceStatus.latitude = (int32_t)(foundLatitude * 1e6);
}

void Generator::SetSatellites()
{
    uint8_t foundSatellites = (uint8_t)randomInt();
    deviceStatus.satellites = foundSatellites;
}

void Generator::SetTemperature()
{
    deviceStatus.temperature = (uint16_t)getRandomDoubleInRange(6, 35);
}

void Generator::SetAltitude()
{
    deviceStatus.altitude = (uint16_t)getRandomDoubleInRange(10, 1500);
}

double Generator::toRadians(double degree)
{
    return degree * (PI / 180.0);
}

double Generator::doubleRand()
{
    return double(rand()) / (double(RAND_MAX) + 1.0);
}

int Generator::randomInt()
{
    return 0 + (std::rand() % (10 - 0 + 1));
}

double Generator::getRandomDoubleInRange(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;

    return fMin + f * (fMax - fMin);
}