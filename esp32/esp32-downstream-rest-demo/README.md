# Esp32 Downstream Rest Demo

This code sample, provides simple guidance on how to get started receiving data 
from the IotBridge platform to your device via Rest.

## Requirements

1. PlatformIO
2. Esp32
3. IotBridge Account & Device

## Getting Started

The first step is to clone the repository. Once that is complete, open the solution in PlatformIO.
You need to update some of the variables in the clearly marked section.

The authorization token can be found in the portal, under device detail, and the communication tab.
This example uses Rest, so use the values under that section.

Once the values have been updated, you can build and upload the code to the device.
The device should start up, connect to your Wifi network and then send a single message to the server.

## Device Magix Profile

You will need to create a profile for this sample. 
The Magix profile is code you need to write, in order to manipulate the outgoing data, 
so that it can be used in your device.

### The encode function would look like this

```
function decode(message, metadata) {
   return message.asString;
}
```

### Actions Configuration

For this example, we added two actions for the device.
Navigate to the device detail page, then under actions click on the 'Add button'

On:
For the name use: On, for the description you can use: Turn the LED on.
No parameters are required, you can click on submit

Off:
For the name use: Off, for the description you can use: Turn the LED off.
No parameters are required, you can click on submit

