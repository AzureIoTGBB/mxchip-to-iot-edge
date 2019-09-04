# Module 3 - Develop and push the MXChip firmware

In this module, we will write and flash the firmware for the MXChip to talk through IoT Edge. The primary trick for this is to get the MXChip to 'trust' the TLS certificate returned from our IoT Edge box when the MXChip firmware tries to open a connection.

Per the iot edge certificate article, the IoT Edge device will return a TLS 'server' certificate that is derived from (i.e. signed by) the root CA certificate that we copied at the end of Module 2.

Through this module, we will add that certificate to the MXChip firmware as the 'trusted root' certificate for our TLS connection.  Let's get started.

## Getting started with the MXChip.

We are going to get started with the (pun intended) 'getting started' project.  To generate that project,open VS Code and connect your MXChip to the USB port on the computer.  The Azure IoT Workbench will launch within VS Code.

TODO:  steps to create the 'getting started' project.

## Customize the MXChip firmware

There are a couple of options for customizing and updating the firmware.  You can either follow along with option 1 below, or just get the pre-written code for you and update/change the root ca certificate, as in option 2 below.

In both cases, you start from the "getting started" sample that comes with the Azure IoT Workbench, which you installed as a pre-requisite in Module 1.  Also in both cases, you need to format your root CA certificate that you copied at the end of Module 2.  So we will start there.

### format the root CA cert for use in the MXChip code

The root CA cert that you copied at the end of module 2 looks something like this (with different values, of course)


```
-----BEGIN CERTIFICATE-----
MIIFdzCCA1+gAwIBAgIJAOE4HEHpwIEgMA0GCSqGSIb3DQEBCwUAMCoxKDAmBgNV
BAMMH0F6dXJlX0lvVF9IdWJfQ0FfQ2VydF9UZXN0X09ubHkwHhcNMTkwOTA0MDA0
MjUwWhcNMTkxMDA0MDA0MjUwWjAqMSgwJgYDVQQDDB9BenVyZV9Jb1RfSHViX0NB
X0NlcnRfVGVzdF9Pbmx5MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA
89mYysDrHlW4tnjLrCrBIomfWSMJFa22JxqjbP5Q23QPgqxi93GBEJYAfpUTzANm
4RTA8KobTLSwoTMCWy0Pxc0SdNJSRwO/Gd6hF9nYWd9VfmVlP68jIsvr9Unqb6xN
oeeWkaTARzkTowtvLItuInaINwMcBOwo7nDKT8V2lgZfEJ0Hvt+kkawRx+dwaAhI
MRrNKXAEC2yN55Srfgqt8ozegop3873f1BVJcUY4L099a1SxFoIufoiFfBjSkCIx
F/oOeUJ6PTsYN3JIY+YXP5xSWHBwEMhpXdda/laJZP8HZFenCx8uCgLebi1e5gaG
goW/Eu1mygp9Zurt3ha20zmeNRvbAva7caUkOliow0FzGmU0eGRu05aViYZ3qhkp
FLETXm1wuVObE1brTF5Br1RnRwXIA3XSl1XhNqlz+oO/gDPAihh9zfbZv1r3lIZR
+zny1ed3BMrB4j4jAIdHYeBzUxJwZ9Udhwlb75jz/f6idzvQZRgCuS9LV0sim84v
ELpefyAuPsYwizbAvk56otJEseWBAs3gNPlEOBHc6ckPCD3pm4czzqhTluS54vAs
r10fonISBSREQQJyCfIsI5MOZztTggamrzmA1bTYfKxJ3GpbsY3kKlbp9VMC8HqQ
DlhNCNJc6B80t3c2edG2UyTFdKqobSgFOIBwuEPPUrMCAwEAAaOBnzCBnDAdBgNV
HQ4EFgQUcqrCtzObsCC7oYd8/fczJXQc5vYwWgYDVR0jBFMwUYAUcqrCtzObsCC7
oYd8/fczJXQc5vahLqQsMCoxKDAmBgNVBAMMH0F6dXJlX0lvVF9IdWJfQ0FfQ2Vy
dF9UZXN0X09ubHmCCQDhOBxB6cCBIDAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB
/wQEAwIBhjANBgkqhkiG9w0BAQsFAAOCAgEAeiQJZFrZR2XwC75ZmRgIjeP+dyX6
TBhDw1sjVqPcVnxrq1PLDrK/AoZGyJHKa3zHMxL/NdXTyQhFpV3xRTlIkxTkU23Z
OrL7YoUXuvW1OVYKaT0yNHbXB/xczbPFPbowibs6beTD/mKWz73MVLfAI1Qg0Dza
e/YEjhlcn6Fp1oQ1ReKDciSgIlG6PZOZlzf4Nu2OHy7ChuwZj6SI5U+Fo9u5n+4X
VnvqSR40uORLKw0HCUIu6C9TUnXeLaP7YQIEfZBDf+POUo/UNy0esmHv2UUxrBWz
NlGbil77jmZRgR9wQTRayu5uas1JbndUBuUVenpl7jdI7nNNRxSZJu7hSrbqQQOy
6r4ytxnFQg45NpGPt2e46TxgNX3wuO6bm/3Z29zxoQlWBbJJE1tGyJb8SD5X14JD
hpi4QKbYnIKEwy4gceyAill57g8G/a3j67SAh1x3qsq2eH0+lu6l5QssmtID9lTK
zKJg5qRpLjdKn2XehXYk3sv94ddjk3nFTu0EELC7S0QcOiVVa+1HNPRRhe6oTb+T
Oi9K1drk91arzYpJbxE8g/gYbdOkQtFQtCb2/XzxHu1se08S3sYP+e7RkhVL3ig+
qnne8z+WnsfgtAG1Sbdk27dIn+pPU9VUb9XG4eyTJaiPKMrVJu+OBqtHC3TZoxob
o3Q7FbIPoLI9Mb4=
-----END CERTIFICATE-----
```

To use in our C code in the MXChip, we need to 'stringify' the CA cert.  For each line, put a " (double quote) at the front of the line.  Then, at the end of each line, add a \r\n"  (carriage return, newline, double quote)

Then it would look like this...

```
"-----BEGIN CERTIFICATE-----\r\n"
"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\r\n"
"o3Q7FbIPoLI9Mb4=\r\n"
"-----END CERTIFICATE-----\r\n";
```

Add a semicolon to the very end as in the example above.  Hang on to that new cert string array, as we are going to need it shortly.

Now we are ready to start coding.  If you haven't already, open up VS Code.  Plug in your MXChip device into your computer's USB port.  The "Examples-Azure IoT Device Workbench" page should launch.  From it, under the "Getting Started" sample, click on "Open Sample"..  that will open a new VS Code instance with the Getting Started sample open.  From here, you can either follow along with the code changes, which are few (option 1), or pick the pre-written code (option 2)

### Option 1

#### getstarted.ino

Open the "GetStarted.ino" file.  Near the top, just below the line

```csharp
static float humidity;
```

Add in the following

```csharp
static const char edgeCert [] =
<<your cert string here>>

```
where \<your cert string here> is the cert string array we just created.

Scroll down to the 'setup()' method and just after this line of code

```csharp
  DevKitMQTTClient_SetOption(OPTION_MINI_SOLUTION_NAME, "DevKit-GetStarted");
```

add this additional line of code

```csharp
  //iot-edge-change:  add the root ca cert to the MQTT 'trusted' certs
  DevKitMQTTClient_SetOption("TrustedCerts", edgeCert);
```

And finally (for this file), down in the 'loop()' method, comment out this line of code as shown

```csharp
//      DevKitMQTTClient_Event_AddProp(message, "temperatureAlert", temperatureAlert ? "true" : "false");
```


