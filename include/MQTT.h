
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <EasyButton.h>
#include <Ticker.h>
#include <pins.h>

#ifndef RED_GARAGE_H
#define RED_GARAGE_H


// MQTT Settings
// debug mode, when true, will send all packets received from the heatpump to topic mqtt_debug_topic
// this can also be set by sending "on" to mqtt_debug_set_topic
bool _debugMode = false;
bool retain = true; //change to false to disable mqtt retain


// prefix for all MQTT topics
const char* mqttTopicPrefix = "garage";

/*
 * ********************************************************************************
 *            END CONFIGURATION SECTION
 * ********************************************************************************
*/



WiFiClient espClient;
PubSubClient mqtt_client(espClient);


// mqtt client settings
char mqtt_topic[64]              = "garage/default";  //contains current settings
char mqtt_temperature_topic[64]  = "garage/default/temperature"; //temperature
char mqtt_outdoortemperature_topic[64]; //outdoor temperature
char mqtt_doorbell_topic[64];           // doorbell switch
char mqtt_garagedoor_topic[64];         // garage door open switch
char mqtt_debug_topic[64]        = "garage/default/debug"; //debug messages
char mqtt_debug_set_topic[64]    = "garage/default/debug/set"; //enable/disable debug messages
int secondsWithoutMQTT;

void configureMQTT();
void configModeCallback(WiFiManager *myWIFI);

#endif