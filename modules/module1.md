
# Module 1 - Setup MXChip development environment

The first step in getting the MXChip connected through Azure IoT Edge is to set up your development environment to allow you to build the MXChip firmware. Unfortunately, we can't just use one of the pre-built MXChip firmwares available on the dev kit site, because we have to tell the firmware how to 'trust' the TLS certificate returned by IoT Edge when you initiate a connection.  Therefore, we must build our own firmware using the C SDK. Don't run away just yet, we'll be giving you to the code to do so :-)

## Install pre-requisites

To setup the development environment for the MXChip, following the instructions provided [here](https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-arduino-iot-devkit-az3166-get-started#prepare-the-development-environment).  Start at the "Prepare the Development Environment" section, but stop when you reach the "Build your first project" section

## Verify MXChip drivers installation

To verify that the ST-Link drivers installed correctly, plug in your MXChip via the USB cable to your computer.  Launch "Device Manager", and navigate to the "Ports (COM & LPT)" section.  Verify that your MXChip shows up as a valid COM port as shown below (your port may differ from COM3)

![MXchip com port](/images/mxchip-com-port.png)

Go ahead and unplug your MXChip.  We'll come back to it later...

That's it.  You are now ready to move on to [Module 2](modules/module2.md)