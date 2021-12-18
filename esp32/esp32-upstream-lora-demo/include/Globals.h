#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>

#define PAYLOAD_BUFFER_SIZE 13 // maximum size of payload block per transmit

typedef struct
{
    int32_t latitude;     // bytes 1 - 4
    int32_t longitude;    // bytes 5 - 8
    uint8_t satellites;   // byte 9
    uint16_t temperature; // bytes 10 - 11
    uint16_t altitude;    // bytes 12 - 13
} deviceStatus_t;

#endif