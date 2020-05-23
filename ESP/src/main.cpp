//To edit this code, go to Config.h 

#include <Arduino.h>

#include <Configs.h>     // configs

#if defined(ESP8266)
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>       
#else
#include <WiFi.h> 
#include <WebServer.h> 
#include <WiFiMulti.h>        
#endif

#include <DNSServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <OTA.h>

//libs to comunication with sinric
#include <WebSocketsClient.h> //  https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries
#include <ArduinoJson.h>      // https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries

#if defined(ESP8266)
ESP8266WiFiMulti WiFiMulti;
#else     
WiFiMulti wifiMulti;
#endif

WebSocketsClient webSocket;
WiFiClient client;

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void initPins()
{

#ifdef RELE1_PIN
  pinMode(RELE1_PIN, OUTPUT);
#endif
#ifdef RELE2_PIN
  pinMode(RELE2_PIN, OUTPUT);
#endif
#ifdef RELE3_PIN
  pinMode(RELE3_PIN, OUTPUT);
#endif
#ifdef RELE4_PIN
  pinMode(relePin4, OUTPUT);
#endif

}

void turnOn(String deviceId)
{

  //  if (deviceId == "5axxxxxxxxxxxxxxxxxxx") // Device ID of first device

#ifdef RELE1_PIN
  if (deviceId == RELE1_ID) // Device ID of first device
  {
    digitalWrite(RELE1_PIN, HIGH);
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
  }
#endif

#ifdef RELE2_PIN
  if (deviceId == RELE2_ID) // Device ID of second device
  {
    digitalWrite(RELE2_PIN, HIGH);
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
  }
#endif

#ifdef RELE3_PIN
  if (deviceId == RELE3_ID) // Device ID of second device
  {
    digitalWrite(RELE3_PIN, HIGH);
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
  }
#endif

#ifdef RELE4_PIN
  if (deviceId == RELE4_ID) // Device ID of second device
  {
    digitalWrite(RELE4_PIN, HIGH);
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
  }
#endif

  else
  {
    Serial.print("Turn on for unknown device id: ");
    Serial.println(deviceId);
  }
}

void turnOff(String deviceId)
{
  #ifdef RELE1_PIN
  if (deviceId == RELE1_ID) // Device ID of first device
  {
    digitalWrite(RELE1_PIN, LOW);
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
  }
  #endif

  #ifdef RELE2_PIN
  if (deviceId == RELE2_ID) // Device ID of first device
  {
    digitalWrite(RELE2_PIN, LOW);
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
  }
  #endif

  #ifdef RELE3_PIN
  if (deviceId == RELE3_ID) // Device ID of first device
  {
    digitalWrite(RELE3_PIN, LOW);
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
  }
  #endif

  #ifdef RELE4_PIN
  if (deviceId == RELE4_ID) // Device ID of first device
  {
    digitalWrite(RELE4_PIN, LOW);
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
  }
  #endif
  

  else
  {
    Serial.print("Turn off for unknown device id: ");
    Serial.println(deviceId);
  }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    isConnected = false;
    Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
    break;
  case WStype_CONNECTED:
  {
    isConnected = true;
    Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
    Serial.printf("Waiting for commands from sinric.com ...\n");
  }
  break;
  case WStype_TEXT:
  {
    Serial.printf("[WSc] get text: %s\n", payload);
    // Example payloads

    // For Switch  types
    // {"deviceId":"xxx","action":"action.devices.commands.OnOff","value":{"on":true}} // https://developers.google.com/actions/smarthome/traits/onoff
    // {"deviceId":"xxx","action":"action.devices.commands.OnOff","value":{"on":false}}

#if ARDUINOJSON_VERSION_MAJOR == 5
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.parseObject((char *)payload);
#endif
#if ARDUINOJSON_VERSION_MAJOR == 6
    DynamicJsonDocument json(1024);
    deserializeJson(json, (char *)payload);
#endif
    String deviceId = json["deviceId"];
    String action = json["action"];

    if (action == "action.devices.commands.OnOff")
    { // Switch
      String value = json["value"]["on"];
      Serial.println(value);

      if (value == "true")
      {
        turnOn(deviceId);
      }
      else
      {
        turnOff(deviceId);
      }
    }
    else if (action == "test")
    {
      Serial.println("[WSc] received test command from sinric.com");
    }
  }
  break;
  case WStype_BIN:
    Serial.printf("[WSc] get binary length: %u\n", length);
    break;
  default:
    break;
  }
}

void setup()
{

  // put your setup code here, to run once:
  Serial.begin(115200);

  initPins();

  //WiFiManager---------------------------------------------------------------------------
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //wifiManager.resetSettings();

  //set custom ip for portal
  //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name

  wifiManager.autoConnect(espName);
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  initOTA();

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);

  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000); // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop()
{
  ArduinoOTA.handle();
  webSocket.loop();

  // put your main code here, to run repeatedly:
  if (isConnected)
  {
    uint64_t now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL)
    {
      heartbeatTimestamp = now;
      webSocket.sendTXT("H");
    }
  }
}