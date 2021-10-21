#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>

#define PAYLOAD_BUFFER_SIZE 12 // maximum size of payload block per transmit

typedef struct
{
    int32_t latitude;     // bytes 1 - 4
    int32_t longitude;    // bytes 5 - 8
    uint16_t temperature; // bytes 9 - 10
    uint16_t altitude;    // bytes 11 - 12
} deviceStatus_t;

#endif