#ifndef _PAYLOADENCODER_H_
#define _PAYLOADENCODER_H_

#include "Globals.h"

class PayloadEncoder
{
public:
    PayloadEncoder(uint8_t size);
    ~PayloadEncoder();

    void reset(void);
    uint8_t getSize(void);
    uint8_t *getBuffer(void);
    void AddDeviceStatus(deviceStatus_t value);

private:
    uint8_t *buffer;
    uint8_t cursor;
    void writeLatLng(double latitude, double longitude);
    void writeUint8(uint8_t i);
    void addByte(uint8_t value);
    void writeUint16(uint16_t i);
    void writeUint32(uint32_t i);
    void uintToBytes(uint64_t i, uint8_t byteSize);
};

extern PayloadEncoder payloadEncoder;

#endif