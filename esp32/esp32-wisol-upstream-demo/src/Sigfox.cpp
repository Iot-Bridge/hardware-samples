#include "SigFox.h"

// Constructor
SigFox::SigFox()
{
    isReady = false;
}

void SigFox::Initialize()
{
    Serial2.begin(9600);
    Serial2.setTimeout(100);
    delay(500);

    TestIsReady();
}

void SigFox::SendFrameMessage(uint8_t *buffer)
{
    if (!isReady)
    {
        Serial.println("Sigfox module is NOT ready. Unable to send");

        return;
    }

    Serial.println("Attempting to send");

    if (wisol_sigfox__send_frame(buffer, 12, true))
    {
        Serial.println("message sent.");
    }
}

bool SigFox::GetIsReady()
{
    return isReady;
}

void SigFox::TestIsReady()
{
    if (wisol_sigfox__ready())
    {
        isReady = true;
        Serial.println("Sigfox module is ready.");

        char buf[32];
        wisol_sigfox__get_id(buf);
        Serial.print("ID=");
        Serial.println(buf);

        wisol_sigfox__get_pac(buf);
        Serial.print("PAC=");
        Serial.println(buf);

        wisol_sigfox__get_firmware_version(buf, sizeof(buf));
        Serial.print("FirmwareVersion=");
        Serial.println(buf);

        wisol_sigfox__get_firmware_vcs_version(buf, sizeof(buf));
        Serial.print("FirmwareVCSVersion=");
        Serial.println(buf);

        wisol_sigfox__get_library_version(buf, sizeof(buf));
        Serial.print("LibraryVersion=");
        Serial.println(buf);
    }
    else
    {
        isReady = false;
        Serial.println("Sigfox module is NOT ready.");
    }
}

void wisol_sigfox__serial_sync(const char *cmd, char *p_response_buf, int sz_response_buf, int timeout)
{
    Serial2.setTimeout(timeout);
    Serial2.print(cmd);
    Serial2.print("\n");
    String moduleResponse = Serial2.readStringUntil('\n');
    strncpy(p_response_buf, moduleResponse.c_str(), sz_response_buf);
    int i = moduleResponse.length() - 1;
    while (i > 0 && (p_response_buf[i] == '\n' || p_response_buf[i] == '\r'))
    {
        p_response_buf[i--] = 0;
    }
}