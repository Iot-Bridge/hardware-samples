#ifndef _SIGFOX_h_
#define _SIGFOX_h_

#include <Arduino.h>
#include <wisol_sigfox.h>

class SigFox
{

public:
    SigFox(); // Constructor
    void Initialize();
    void SendFrameMessage(uint8_t *buffer);
    void TestIsReady();
    bool GetIsReady();

private:
    bool isReady;
};

#endif