# Esp32 Upstream Sigfox (SFM10R1) Demo

This code sample, provides simple guidance on how to get started sending data 
from your device to the IotBridge platform via Sigfox using the SFM10R1 module.

## Requirements

1. PlatformIO
2. Esp32
3. IotBridge Account & Device
4. SFM10R1 Module [Link](https://partners.sigfox.com/products/sfm10r1)

## Getting Started

Follow the manufacturer guidelines to enable and register the device with the Sigfox backend.
Connect the SFM10R1 module and the ESP as shown in the diagram below.

![Schematic](Schematic.png)

Next is to clone the repository. Once that is complete, open the solution in PlatformIO.
You can simply build and upload the firmware to the device.

At this point you can log into the Sigfox backend and verify on the device that it is receiving messages.
If you have not already linked your Sigfox account in the IotBridge platform, follow this guide in our docs. [Link](https://docs.iotbridge.co.za/#/accounts-and-subscriptions/adding-sigfox-credentials)

## Device Magix Profile

You will need to create a profile for this sample. 
The Magix profile is code you need to write, in order to manipulate the incoming data, 
so that it can be used in our Platform.

### The decode function would look like this

```
var bytesToInt = function (bytes) {
    var i = 0;
    for (var x = 0; x < bytes.length; x++) {
        i |= (bytes[x] << (x * 8));
    }
    return i;
};

var uint8 = function (bytes) {
    if (bytes.length !== uint8.BYTES) {
        throw new Error('uint8 must have exactly 1 byte');
    }
    return bytesToInt(bytes);
};
uint8.BYTES = 1;

var int16 = function (bytes) {
    if (bytes.length !== int16.BYTES) {
        throw new Error('int16 must have exactly 2 bytes');
    }
    var value = +(bytesToInt(bytes));
    if (value > 32767) {
        value -= 65536;
    }
    return value;
};
int16.BYTES = 2;

var int32 = function (bytes) {
    if (bytes.length !== int32.BYTES) {
        throw new Error('int32 must have exactly 4 bytes');
    }
    var value = +(bytesToInt(bytes));
    if (value > 2147483647) {
        value -= 4294967296;
    }
    return value;
};
int32.BYTES = 4;

var latLng = function (bytes) {
    return +(int32(bytes) / 1e6).toFixed(6);
};
latLng.BYTES = int32.BYTES;

var temp = function (bytes) {
    return +(int16(bytes));
};
temp.BYTES = int16.BYTES;

var altitude = function (bytes) {
    return +(int16(bytes));
};
altitude.BYTES = int16.BYTES;

var decodeInput = function (bytes, mask, names) {

    var maskLength = mask.reduce(function (prev, cur) {
        return prev + cur.BYTES;
    }, 0);
    if (bytes.length < maskLength) {
        throw new Error('Mask length is ' + maskLength + ' whereas input is ' + bytes.length);
    }

    names = names || [];
    var offset = 0;
    return Array.from(mask)
        .map(function (decodeFn) {
            var current = bytes.slice(offset, offset += decodeFn.BYTES);
            return decodeFn(current);
        })
        .reduce(function (prev, cur, idx) {
            prev[names[idx] || idx] = cur;
            return prev;
        }, {});
};

function DecodeHexStringToByteArray(hexString) {
   var result = [];
   while (hexString.length >= 2) { 
       result.push(parseInt(hexString.substring(0, 2), 16));
       hexString = hexString.substring(2, hexString.length);
   }
   return result;
}

function decode(message, metadata)
{
    var data = {};
    var input = DecodeHexStringToByteArray(message.asString);

   data = decodeInput(input, [latLng, latLng, temp, altitude], ['latitude', 'longitude', 'temp', 'altitude']);

  return data;
}
```

### The convert function would look like this

```
function convert(message, metadata)
{
    return {
        $location:{
            latitude: message.latitude,
            longitude: message.longitude
        },
        $telemetry:{
            altitude: message.altitude,
            temperatureC: message.temp,
            temperatureF: (message.temp * 1.8) + 32
        }
    }
}
```

