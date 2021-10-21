#include <Arduino.h>

#include "Generator.h"
#include "PayloadEncoder.h"
#include "Sigfox.h"

#define transmitLedPin 2

Generator myGenerator;
SigFox sigFoxModule;
PayloadEncoder payloadEncoder(PAYLOAD_BUFFER_SIZE);

unsigned long lastCheck = 0;
unsigned long interval = 60000 * 15; // 10 Minutes

void setup()
{
  Serial.begin(115200);
  delay(500);

  pinMode(transmitLedPin, OUTPUT);

  sigFoxModule.Initialize();

  lastCheck = interval;
}

void loop()
{
  if ((millis() - lastCheck) > interval)
  {
    lastCheck = millis();

    if (!sigFoxModule.GetIsReady())
    {
      sigFoxModule.TestIsReady();
    }

    digitalWrite(transmitLedPin, HIGH);
    deviceStatus_t randomDeviceStatus = myGenerator.GetRandomStatus();
    payloadEncoder.reset();
    payloadEncoder.AddDeviceStatus(randomDeviceStatus);

    uint8_t *payloadBuffer = payloadEncoder.getBuffer();
    sigFoxModule.SendFrameMessage(payloadBuffer);
    digitalWrite(transmitLedPin, LOW);
  }
}