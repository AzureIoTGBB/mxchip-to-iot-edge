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
where \<your cert string here> is the cert string array we just created. This line creates a string array variable that contains our root CA certificate (from which the edge TLS server cert is signed).

Scroll down to the 'setup()' method and just after this line of code

```csharp
  DevKitMQTTClient_SetOption(OPTION_MINI_SOLUTION_NAME, "DevKit-GetStarted");
```

add this additional line of code

```csharp
  //iot-edge-change:  add the root ca cert to the MQTT 'trusted' certs
  DevKitMQTTClient_SetOption("TrustedCerts", edgeCert);
```

This line tells the MQTT SDK client (and the underlying MBED library) to add this certificate to it's list of trusted root certificates. And finally (for this file), down in the 'loop()' method, comment out this line of code as shown

```csharp
//      DevKitMQTTClient_Event_AddProp(message, "temperatureAlert", temperatureAlert ? "true" : "false");
```

This line was originally used to add an application property to the outgoing JSON message.  We aren't going to use it, and it's just noise, so we drop it.

#### utility.cpp

Now open the utility.cpp file.  Unfortunately, the Getting Started sample only samples from two of the many sensors on the MXChip. We are going to update the code to read from the other sensors.

At the top of the file, below the line

```csharp
DevI2C *i2c;
```

declare variables to hold the additional sensors

```csharp
static HTS221Sensor *sensor;
static LSM6DSLSensor *accelGyro;
static LIS2MDLSensor *magnetometer;
static LPS22HBSensor *pressure;
```

Beneath the readHumidy() method, add the following additional methods to read the other sensors

```csharp
//iot-edge-change:add method to read pressure
float readPressure()
{

//     assert(pressure != NULL);
    if (pressure == NULL) {
        LogError("Trying to do readPressure while the sensor wasn't initialized.");
        return 0xFFFF;
    }

    float presureValue;
    if (pressure->getPressure(&presureValue) == 0)
        return presureValue;
    else
        return 0xFFFF;   
}
//iot-edge-change: add method to read magnetometer
void readMagnetometer(int *axes)
{
        bool hasFailed = false;

//    assert(magnetometer != NULL);
    if (magnetometer == NULL) {
        LogError("Trying to do readMagnetometer while the sensor wasn't initialized.");
        hasFailed = true;
    }
    else if (magnetometer->getMAxes(axes) != 0) {
        hasFailed = true;
    }

    if (hasFailed) {
        axes[0] = 0xFFFF;
        axes[1] = 0xFFFF;
        axes[2] = 0xFFFF;
    }
}

//iot-edge-change: add method to read Accelerometer
void readAccelerometer(int *axes)
{
        bool hasFailed = false;

//    assert(magnetometer != NULL);
    if (magnetometer == NULL) {
        LogError("Trying to do readMagnetometer while the sensor wasn't initialized.");
        hasFailed = true;
    }

    if (accelGyro->getXAxes(axes) != 0) {
        hasFailed = true;
    }

    if (hasFailed) {
        axes[0] = 0xFFFF;
        axes[1] = 0xFFFF;
        axes[2] = 0xFFFF;
    }
}

//iot-edge-change:  add method to read Gyroscope
void readGyroscope(int *axes)
{
        bool hasFailed = false;

//    assert(accelGyro != NULL);
    if (accelGyro == NULL) {
        LogError("Trying to do readGyroscope while the sensor wasn't initialized.");
        hasFailed = true;
    }

    if (accelGyro->getGAxes(axes) != 0) {
        hasFailed = true;
    }

    if (hasFailed) {
        axes[0] = 0xFFFF;
        axes[1] = 0xFFFF;
        axes[2] = 0xFFFF;
    }
}
```

select the readMessage() function and replace it with this one. We'll discuss the changes below

```csharp
bool readMessage(int messageId, char *payload, float *temperatureValue, float *humidityValue)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;

    json_object_set_number(root_object, "messageId", messageId);

    float t = readTemperature();
    float h = readHumidity();
    bool temperatureAlert = false;
//    if(t != temperature)
//    {
        temperature = t;
        *temperatureValue = t;
        json_object_set_number(root_object, "temperature", temperature);
//    }
//    if(temperature > TEMPERATURE_ALERT)
//    {
//        temperatureAlert = true;
//    }
    
//    if(h != humidity)
//    {
        humidity = h;
        *humidityValue = h;
        json_object_set_number(root_object, "humidity", humidity);
//    }

    int magAxes[3];
    readMagnetometer(magAxes);
    json_object_set_number(root_object, "magnetometerX", magAxes[0]);
    json_object_set_number(root_object, "magnetometerY", magAxes[1]);
    json_object_set_number(root_object, "magnetometerZ", magAxes[2]);

    int accelAxes[3];
    readAccelerometer(accelAxes);
    json_object_set_number(root_object, "accelerometerX", accelAxes[0]);
    json_object_set_number(root_object, "accelerometerY", accelAxes[1]);
    json_object_set_number(root_object, "accelerometerZ", accelAxes[2]);

    int gyroAxes[3];
    readGyroscope(accelAxes);
    json_object_set_number(root_object, "gyroscopeX", gyroAxes[0]);
    json_object_set_number(root_object, "gyroscopeY", gyroAxes[1]);
    json_object_set_number(root_object, "gyroscopeZ", gyroAxes[2]);


//    serialized_string = json_serialize_to_string_pretty(root_value);
    serialized_string = json_serialize_to_string(root_value);

    snprintf(payload, MESSAGE_MAX_LEN, "%s", serialized_string);
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
    return temperatureAlert;
}
```

The original function only sent temperature and humidity values if they changed.  For our lab, we want to send the values every time, so we commented out the 'if' statements that surrouned them.   Next, we added calls to read the accelerometer, magnetometer, and gyroscope sensors and append those values onto our JSON object.  Finally, the json_serial_to_string_pretty function serializes the sensor readings to JSON, but adds carriage returns and line feeds into the string to make it "pretty" when it prints out.  We don't want all that extra stuff in our JSON, so we use the non-'pretty' version of the call.

Save all the files (CTRL-K S).

If you went this option, feel free to review the rest of the code to understand what it is doing, and then skip down to the "build and flash firmware" section.

#### Option 2

If you chose to just use the existing code, feel free to look through it and see the changes we made.  Each one is proceeded by a "//iot-edge-change" comment.

We do have to make one change to the code, and that is to, at the top of the GetStarted.ino file, to add our cert. So open that file and at the top, just below the line

```csharp
static const char edgeCert [] =
```
replace the cert string array with the one you generated above.  Save all the files (CTRL-K S).

### Build and flash the firmware

To bulid the firmware, first we need to 'verify' it  (in 'arduino' speak, that's the same as 'compile').  To do so, hit CTRL-ALT-R.  That will compile the code and tell if you have any errors.

Once that is done, we are ready to push the new fireware to the MXChip.  In the bottom right hand of VS Code, click on the "<Select Board Type>" button and pick "TODO:  get board type".  Also, if you still see "<Select Serial Port>", click on that to select the right serial port that represents your plugged in MXChip.   

To push the firmware, hit CTRL-ALT-U (for 'upload').  You'll see the code get 'verified' again, and then see it pushed to the MXChip device.  You'll then see the MXChip device reset itself and start to boot.

However, we aren't quite ready to connect to IoT Edge yet, because we need to configure our MXChip.

###  Configure your MXChip for connectivity to IoT Edge

The first thing we need to do is to decorate our MXChip device connection string.  We need to tell the Azure IoT C SDK that we want to connect to IoT Hub, but we want to do so *through* your IoT Edge box.  To do that, take the MXChip device connection string, and onto the end of it, append a semicolon, then the phrase 'GatewayHostName=' and then the FQDN to your IoT Edge box.  An example might look like this:

TODO:  sample mxchip connection string.

Once that is done, we are ready to configure our MXChip.

TODO:   finish configuration commands....


### Test connectivity to IoT Hub thorugh IoT Edge.

Before we restart our MXChip, let's set up some monitoring

In the Azure Portal, in your cloud shell, run this command

```bash
az iot hub monitor-events -n [iot hub name] -t 0
```

That will monitor the IoT Hub for messages flowing into it from the MXChip

In your Putty/SSH session to your IoT Edge VM, run

```bash
sudo docker logs -f edgeHub --tail 500
```

That will show the last few lines of the edgeHub (part of the IoT Edge runtime) logs and will 'follow' them (i.e. see new entries as they arrive)

TODO:  screenshots

Now, hit the reset button on your MXChip.  In the MXChip putty session, you should see output showing the MXChip start, get the current time from an NTP server, connect to IoT Edge, and then start sending data.  Similar to this screenshot

in the edgeHub logs, you should see entries related to the MXChip device id connecting and edgeHub opening a connection to IoT Hub on behalf of the MXChip, similar to below

Finally, you should see messages flowing into the IoT Hub via the cloud shell in the azure portal.

Note that the messages are flowing in at a rate of 1 per sec.  In the next module, we'll do some light "edge processing" to aggregate those messages "on-prem"  (remember, we are simulating that) before they are sent to Azure.   So let's move on to [Module 4](modules/module4.md)


