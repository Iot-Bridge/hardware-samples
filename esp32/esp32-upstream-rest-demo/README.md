# Esp32 Upstream Rest Demo

This code sample, provides simple guidance on how to get started sending data 
from your device to the IotBridge platform via Rest.

## Requirements

1. PlatformIO
2. Esp32
3. IotBridge Account & Device [Link](https://www.iotbridge.co.za)

## Getting Started

The first step is to clone the repository. Once that is complete, open the solution in PlatformIO.
You need to update some of the variables in the clearly marked section.

The authorization token can be found in the portal, under device detail, and the communication tab.
This example uses Rest, so use the values under that section.

Once the values have been updated, you can build and upload the code to the device.
The device should start up, connect to your Wifi network and then send a single message to the server.

## Device Magix Profile

You will need to create a profile for this sample. 
The Magix profile is code you need to write, in order to manipulate the incoming data, 
so that it can be used in our Platform.

### The decode function would look like this

```javascript
function decode(message, metadata) {
   return message.asString;
}
```

### The convert function would look like this

```javascript
function convert(message, metadata)
{
    return {
        $location: {
            longitude: message.longitude,
            latitude: message.latitude
        },
        $telemetry: {
            sats: message.satellites,
            temp: message.temperature,
            altitude: message.altitude
        }
    };
}
```

