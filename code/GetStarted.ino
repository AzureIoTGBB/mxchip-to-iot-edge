// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 
// To get started please visit https://microsoft.github.io/azure-iot-developer-kit/docs/projects/connect-iot-hub?utm_source=ArduinoExtension&utm_medium=ReleaseNote&utm_campaign=VSCode
#include "AZ3166WiFi.h"
#include "AzureIotHub.h"
#include "DevKitMQTTClient.h"

#include "config.h"
#include "utility.h"
#include "SystemTickCounter.h"

static bool hasWifi = false;
int messageCount = 1;
int sentMessageCount = 0;
static bool messageSending = true;
static uint64_t send_interval_ms;

static float temperature;
static float humidity;

// iot-edge-change:   Add iot edge root ca cert
static const char edgeCert [] =
"-----BEGIN CERTIFICATE-----\r\n"
"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\r\n"
"-----END CERTIFICATE-----";

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
static void InitWifi()
{
  Screen.print(2, "Connecting...");
  
  if (WiFi.begin() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    Screen.print(1, ip.get_address());
    hasWifi = true;
    Screen.print(2, "Running... \r\n");
  }
  else
  {
    hasWifi = false;
    Screen.print(1, "No Wi-Fi\r\n ");
  }
}

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    blinkSendConfirmation();
    sentMessageCount++;
  }

  Screen.print(1, "> IoT Hub");
  char line1[20];
  sprintf(line1, "Count: %d/%d",sentMessageCount, messageCount); 
  Screen.print(2, line1);

  char line2[20];
  sprintf(line2, "T:%.2f H:%.2f", temperature, humidity);
  Screen.print(3, line2);
  messageCount++;
}

static void MessageCallback(const char* payLoad, int size)
{
  blinkLED();
  Screen.print(1, payLoad, true);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == NULL)
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  parseTwinMessage(updateState, temp);
  free(temp);
}

static int  DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  LogInfo("Try to invoke method %s", methodName);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    LogInfo("Start sending temperature and humidity data");
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    LogInfo("Stop sending temperature and humidity data");
    messageSending = false;
  }
  else
  {
    LogInfo("No method %s found", methodName);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino sketch
void setup()
{
  Screen.init();
  Screen.print(0, "IoT DevKit");
  Screen.print(2, "Initializing...");
  
  Screen.print(3, " > Serial");
  Serial.begin(115200);

  // Initialize the WiFi module
  Screen.print(3, " > WiFi");
  hasWifi = false;
  InitWifi();
  if (!hasWifi)
  {
    return;
  }

  LogTrace("HappyPathSetup", NULL);

  Screen.print(3, " > Sensors");
  SensorInit();

  Screen.print(3, " > IoT Hub");
  DevKitMQTTClient_SetOption(OPTION_MINI_SOLUTION_NAME, "DevKit-GetStarted");
  
  //iot-edge-change:  add the root ca cert to the MQTT 'trusted' certs
  DevKitMQTTClient_SetOption("TrustedCerts", edgeCert);
  
  DevKitMQTTClient_Init(true);
  DevKitMQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  DevKitMQTTClient_SetMessageCallback(MessageCallback);
  DevKitMQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  DevKitMQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);

  send_interval_ms = SystemTickCounterRead();
}

void loop()
{
  if (hasWifi)
  {
    if (messageSending && 
        (int)(SystemTickCounterRead() - send_interval_ms) >= getInterval())
    {
      // Send teperature data
      char messagePayload[MESSAGE_MAX_LEN];

      bool temperatureAlert = readMessage(messageCount, messagePayload, &temperature, &humidity);
      LogInfo(messagePayload);
      EVENT_INSTANCE* message = DevKitMQTTClient_Event_Generate(messagePayload, MESSAGE);

// iot-edge-change:   comment out this property      
//      DevKitMQTTClient_Event_AddProp(message, "temperatureAlert", temperatureAlert ? "true" : "false");

      DevKitMQTTClient_SendEventInstance(message);
      
      send_interval_ms = SystemTickCounterRead();
    }
    else
    {
      DevKitMQTTClient_Check();
    }
  }
  delay(1000);
}
