#include "PayloadEncoder.h"

PayloadEncoder::PayloadEncoder(uint8_t size)
{
    buffer = (uint8_t *)malloc(size);
    cursor = 0;
}

PayloadEncoder::~PayloadEncoder(void) { free(buffer); }

void PayloadEncoder::reset(void) { cursor = 0; }

uint8_t PayloadEncoder::getSize(void) { return cursor; }

uint8_t *PayloadEncoder::getBuffer(void) { return buffer; }

void PayloadEncoder::addByte(uint8_t value) { writeUint8(value); }

void PayloadEncoder::writeLatLng(double latitude, double longitude)
{
    writeUint32(latitude);
    writeUint32(longitude);
}

void PayloadEncoder::writeUint8(uint8_t i) { uintToBytes(i, 1); }
void PayloadEncoder::writeUint16(uint16_t i) { uintToBytes(i, 2); }
void PayloadEncoder::writeUint32(uint32_t i) { uintToBytes(i, 4); }

void PayloadEncoder::uintToBytes(uint64_t value, uint8_t byteSize)
{
    for (uint8_t x = 0; x < byteSize; x++)
    {
        byte next = 0;
        if (sizeof(value) > x)
        {
            next = static_cast<byte>((value >> (x * 8)) & 0xFF);
        }
        buffer[cursor] = next;
        ++cursor;
    }
}

void PayloadEncoder::AddDeviceStatus(deviceStatus_t value)
{
    writeLatLng(value.latitude, value.longitude);
    writeUint8(value.satellites);
    writeUint16(value.temperature);
    writeUint16(value.altitude);
}