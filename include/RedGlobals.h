#include <dConsole.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <EasyButton.h>

#ifndef RED_GLOBALS_H
#define RED_GLOBALS_H

/*
 * ********************************************************************************
 *            START CONFIGURATION SECTION
 * ********************************************************************************
*/

#define _TEMP_SENSOR_PERIOD 60000 // in ms the frequency of temperature sensor reading
#define TEMPERATURE_PRECISION 9   // Lower resolution
#define _DISPLAY_INTERVAL 5000    // in ms how long before the display is dimmed then turned off
#define GARAGEDOOR_MOVE_TIME 20   // twice (2x) the time it take the garage door to open (in seconds)

/*********************************************************************************
 * 

  Hardware configuration:

12/18/20 V1.0 -- Initial release
12/23/20 V1.1 -- Added controls on moving garage door
4/16/21 V1.2PIO -- Moved to Platform IO


 * ********************************************************************************
 */
#define VERSION "V1.3-PIO"
#define MQTT_TOPIC_PREFIX "led" // prefix for all MQTT topics

// configuration parameters
// Hostname, AP name & MQTT clientID

//define your default values here, if there are different values in config.json, they are overwritten.


// in WiFiConfigurations.ino
extern char myHostName[];
extern char deviceLocation[];
extern char mqttServer[];
extern char mqttPort[];
extern char mqttUser[];
extern char mqttPwd[];
void configureESP(); // load configuration from FLASH & configure WIFI
void checkConnection();   // check WIFI connection
void writeConfigToDisk();
void configureOTA(char *hostName);

// in MQTT
extern PubSubClient mqtt_client;
extern char mqtt_topic[];
extern char mqtt_temperature_topic[];
extern char mqtt_outdoortemperature_topic[];
extern char mqtt_doorbell_topic[];
extern char mqtt_garagedoor_topic[];
extern char mqtt_debug_topic[];
extern char mqtt_debug_set_topic[];
void configureMQTT();
bool checkMQTTConnection();
void mqttDisconnect();
void mqttCallback(char *topic, byte *payload, unsigned int length);

// in console.ino
extern dConsole console;
void setupConsole();
void handleConsole();

// in Sensors.ino
void configSensors(long interval, void (*sensorCallback)(float insideTemp, float outsideTemp));
void serviceSensors();

// in RedGarage
void toggleGarageDoor();
void doorBellButtonPressed();
void updateTemperature(float temp, float outdoorTemp);
void checkDoorBellButton();
void turnOffGarageDoor();


#endif
