#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>

//------------ Configure These Values ---------------------------------//
const char *ssid = "your-ssid-here";              // your network SSID (name of wifi network)
const char *password = "your-wifi-password-here"; // your network password
const String authorizationToken = "your-rest-token-here";
//---------------------- End ------------------------------------------//

const String serverDownstream = "https://rest.iotbridge.co.za/api/downstream";

// The current IotBridge rest channel service root certificate
const char *test_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
    "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
    "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
    "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
    "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
    "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
    "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
    "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
    "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
    "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
    "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
    "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
    "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
    "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
    "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
    "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
    "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
    "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
    "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
    "-----END CERTIFICATE-----\n";

const int ledPin = 23;
DynamicJsonDocument RecieveData();
void ProcessDownstreamMessages(JsonArray downStreamMessages);

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }

  DynamicJsonDocument receivedData = RecieveData();

  if (!receivedData.isNull())
  {
    JsonArray items = receivedData.as<JsonArray>();
    if (items.size() == 0)
    {
      Serial.println("No messages were found");
    }
    else
    {
      ProcessDownstreamMessages(items);
    }
  }
  else
  {
    Serial.println("Something seems to have gone wrong");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

DynamicJsonDocument RecieveData()
{
  if (authorizationToken.length() < 10)
  {
    Serial.println("Please provider an authorization token");

    return DynamicJsonDocument(0);
  }

  DynamicJsonDocument result(2048); // 2KB

  WiFiClientSecure *client = new WiFiClientSecure();
  client->setCACert(test_root_ca);

  if (client)
  {

    HTTPClient httpClient;
    httpClient.setTimeout(15000);

    if (httpClient.begin(*client, serverDownstream))
    {
      httpClient.addHeader("content-type", "application/json");
      httpClient.addHeader("Authorization", "Bearer " + authorizationToken);

      int httpResponseCode = httpClient.GET();
      if (httpResponseCode > 0)
      {
        String responseString = httpClient.getString();

        if (httpResponseCode == HTTP_CODE_OK)
        {
          DeserializationError error = deserializeJson(result, responseString);
          if (error)
          {
            Serial.println("Deserialization Failed, Response Code: " + String(httpResponseCode) + " Response String: " + responseString);
          }

          Serial.println(responseString);
        }
        else
        {
          Serial.println("Response Code: " + String(httpResponseCode) + " Response String: " + responseString);
        }
      }
      else
      {
        Serial.println("GET Failed, Response Code: " + String(httpResponseCode) + " Response String: " + httpClient.errorToString(httpResponseCode));
      }

      httpClient.end();
    }
    else
    {
      Serial.println("Could Not Connect");
    }

    delete client;
  }
  else
  {
    Serial.println("Could Not Create Client");
  }

  return result;
}

void ProcessDownstreamMessages(JsonArray downStreamMessages)
{
  for (JsonVariant downstreamEntry : downStreamMessages)
  {
    String downstreamMessage = downstreamEntry.as<String>();

    Serial.println("Processing Downstream - " + downstreamMessage);

    if (downstreamMessage.indexOf("On") > 0)
    {
      digitalWrite(ledPin, HIGH);
    }
    else if (downstreamMessage.indexOf("Off") > 0)
    {
      digitalWrite(ledPin, LOW);
    }
  }
}
