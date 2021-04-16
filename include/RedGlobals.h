#include <dConsole.h>

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
const char *version = "V1.3-PIO";

dConsole console;

// configuration parameters
// Hostname, AP name & MQTT clientID
char myHostName[64];

//define your default values here, if there are different values in config.json, they are overwritten.
char deviceLocation[64] = "NEW";
char mqttServer[64] = "MQTT";
char mqttPort[16] = "1883";
char mqttUser[64] = "";
char mqttPwd[64] = "";


// in console.ino
void setupConsole();
void handleConsole();

// in WiFiConfigurations.ino
void configureESP(); // load configuration from FLASH & configure WIFI
void checkConnection();   // check WIFI connection
void writeConfigToDisk();

// in Sensors.ino
void configSensors(long interval, void (*sensorCallback)(float insideTemp, float outsideTemp));
void serviceSensors();

// garage
void toggleGarageDoor();
bool checkMQTTConnection();

#endif
