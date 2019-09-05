// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 

#include "HTS221Sensor.h"
//steve 
#include "LSM6DSLSensor.h"
#include "LIS2MDLSensor.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "IrDASensor.h"

#include "AzureIotHub.h"
#include "Arduino.h"
#include "parson.h"
#include "config.h"
#include "RGB_LED.h"

#define RGB_LED_BRIGHTNESS 32

DevI2C *i2c;
//iot-edge-change:  declare variables for other sensors
static HTS221Sensor *sensor;
static LSM6DSLSensor *accelGyro;
static LIS2MDLSensor *magnetometer;
static LPS22HBSensor *pressure;

static RGB_LED rgbLed;
static int interval = INTERVAL;
static float humidity;
static float temperature;

int getInterval()
{
    return interval;
}

void blinkLED()
{
    rgbLed.turnOff();
    rgbLed.setColor(RGB_LED_BRIGHTNESS, 0, 0);
    delay(500);
    rgbLed.turnOff();
}

void blinkSendConfirmation()
{
    rgbLed.turnOff();
    rgbLed.setColor(0, 0, RGB_LED_BRIGHTNESS);
    delay(500);
    rgbLed.turnOff();
}

void parseTwinMessage(DEVICE_TWIN_UPDATE_STATE updateState, const char *message)
{
    JSON_Value *root_value;
    root_value = json_parse_string(message);
    if (json_value_get_type(root_value) != JSONObject)
    {
        if (root_value != NULL)
        {
            json_value_free(root_value);
        }
        LogError("parse %s failed", message);
        return;
    }
    JSON_Object *root_object = json_value_get_object(root_value);

    double val = 0;
    if (updateState == DEVICE_TWIN_UPDATE_COMPLETE)
    {
        JSON_Object *desired_object = json_object_get_object(root_object, "desired");
        if (desired_object != NULL)
        {
            val = json_object_get_number(desired_object, "interval");
        }
    }
    else
    {
        val = json_object_get_number(root_object, "interval");
    }
    if (val > 500)
    {
        interval = (int)val;
        LogInfo(">>>Device twin updated: set interval to %d", interval);
    }
    json_value_free(root_value);
}

void SensorInit()
{
    i2c = new DevI2C(D14, D15);
    sensor = new HTS221Sensor(*i2c);
    sensor->init(NULL);

    accelGyro = new LSM6DSLSensor(*i2c, D4, D5);
    accelGyro->init(NULL);
    accelGyro->enableAccelerator();
    accelGyro->enableGyroscope();

    // enable double tap detection
    accelGyro->enablePedometer();
    accelGyro->setPedometerThreshold(LSM6DSL_PEDOMETER_THRESHOLD_MID_LOW);

    // LIS2MDL
    magnetometer = new LIS2MDLSensor(*i2c);
    magnetometer->init(NULL);

    // LPS22HB
    pressure = new LPS22HBSensor(*i2c);
    pressure->init(NULL);

    humidity = -1;
    temperature = -1000;
}

float readTemperature()
{
    sensor->reset();

    float temperature = 0;
    sensor->getTemperature(&temperature);

    return temperature;
}

float readHumidity()
{
    sensor->reset();

    float humidity = 0;
    sensor->getHumidity(&humidity);

    return humidity;
}

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


//iot-edge-change:  made lots of changes to this function....
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

#if (DEVKIT_SDK_VERSION >= 10602)
void __sys_setup(void)
{
    // Enable Web Server for system configuration when system enter AP mode
    EnableSystemWeb(WEB_SETTING_IOT_DEVICE_CONN_STRING);
}
#endif