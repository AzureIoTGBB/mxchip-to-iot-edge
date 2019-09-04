# Module 3 - Develop and push the MXChip firmware

In this module, we will write and flash the firmware for the MXChip to talk through IoT Edge. The primary trick for this is to get the MXChip to 'trust' the TLS certificate returned from our IoT Edge box when the MXChip firmware tries to open a connection.

Per the iot edge certificate article, the IoT Edge device will return a TLS 'server' certificate that is derived from (i.e. signed by) the root CA certificate that we copied at the end of Module 2.

Through this module, we will add that certificate to the MXChip firmware as the 'trusted root' certificate for our TLS connection.  Let's get started.

## Getting started with the MXChip.

We are going to get started with the (pun intended) 'getting started' project.  To generate that project,open VS Code and connect your MXChip to the USB port on the computer.  The Azure IoT Workbench will launch within VS Code.

TODO:  steps to create the 'getting started' project.

## Customize the MXChip firmware


