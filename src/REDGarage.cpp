/************************************************************************************

   Red Garage - indoor and outdoor temperature sensor, garage Door opener and door bell

   Configuration parameters are found in .h file

  This code implements various garage functionality including:

    1. Indoor and outdoor temp sensors
    2. Garage door opener
    3. House Door Bell
    4. interfaces with OpenHab through MQTT

  This code provides WIFI & MQTT configuration through the use of WIFI manager. Initial setup allow
  configuration of WIFI network and MQTT server

  It provides OTA functionality.

 *************************************************************************************/
#include <Arduino.h>
#include <pins.h>
#include <RedGlobals.h>


//for LED status
Ticker ticker;

// for Button callback
Ticker doorBellDelay;

// for Garage door relay
Ticker garageDoorDelay;

EasyButton doorBellButton(pgm_pin);

void tick()
{
  //toggle state
  int state = digitalRead(blueLED); // get the current state of GPIO1 pin
  digitalWrite(blueLED, !state);    // set pin to the opposite state
}

/*
 * ********************************************************************************

   Perform the initial hardware setup and then sequence the starting of the
   various modules

 * ********************************************************************************
*/

void setup() {
  pinMode(blueLED, OUTPUT);
  pinMode(GARAGE_DOOR, OUTPUT);
  digitalWrite(GARAGE_DOOR, LOW);




  doorBellButton.begin();
  doorBellButton.onPressed(doorBellButtonPressed);
  
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  setupConsole();

  //console.enableSerial(&Serial, true);
  console.print("[RED]Garage ");
  console.println(VERSION);


  // Configure WIFI
  configureESP(); // load configuration from FLASH & configure WIFI

  digitalWrite(blueLED, LOW);
  console.enableTelnet(23);
  console.print("Telnet Enabled on ");
  console.println(WiFi.localIP().toString());

  // configure MQTT topics & connect
  configureMQTT();


  // configure temp sensor
  configSensors(_TEMP_SENSOR_PERIOD, &updateTemperature);

}


/*
 * ********************************************************************************

   main loop services all modules: Wifi, MQTT, HP, console and webserver

 * ********************************************************************************
*/
void loop() {

  checkConnection();  // check WIFI connection

  serviceSensors(); // service temperature and other sensos

  doorBellButton.read(); // service the buttons

  checkMQTTConnection();  // check MQTT

  handleConsole(); // handle any commands from console

}

/*
 * ********************************************************************************

   Button control rotines

 * ********************************************************************************
*/

void doorBellButtonPressed()
{
  console.println("Button: bell");
  mqtt_client.publish(mqtt_doorbell_topic, "ON");

  // check in 2 seconds to clear the state
  doorBellDelay.attach(2, checkDoorBellButton);

}


// once pressed check every 10 seconds until it is released
void checkDoorBellButton()
{
  if (doorBellButton.isReleased())
  {
    doorBellDelay.detach();
    console.println("Doorbell has been released");
    mqtt_client.publish(mqtt_doorbell_topic, "OFF");

  }
}


/*
 * ********************************************************************************

   This is callback routine that is called when the temperature sensor receives
   a reading. It collects the data into a long term average then,
   every _TEMP_SENSOR_PERIOD report the value back to the MQTT server

 * ********************************************************************************
*/
void updateTemperature(float temp, float outdoorTemp)
{
  char str[128];
  console.println("Reporting temp reading (in/out) of " + String(temp) + " and " + String(outdoorTemp));
  if (temp > -100)
  {
    sprintf(str, "%.1f", temp);
    mqtt_client.publish(mqtt_temperature_topic, str);
  }
  if (outdoorTemp > -100)
  {
    sprintf(str, "%.1f", outdoorTemp);
    mqtt_client.publish(mqtt_outdoortemperature_topic, str);
  }

  // we have
  if ( (temp <= -100) || (outdoorTemp <= -100) ) 
  {
    sprintf(str,"Sensor Error: #1 = %.1f #2 = %.1f",temp, outdoorTemp);
    mqtt_client.publish(mqtt_debug_topic,str);
  }

  tick();

}

// because the garage door open/close signal is momentary, and we have no feedback
// as to whether the door is open or closed, we have to be very carefull on how
// the signal is activated.
// 1. NEVER activate it without a person present to give you feedback
// 2. once the garage door received a "1" (ON) command, it goes on for approximately
// twice the time it takes the garage door to open. No commands are accepted during this
// period.

bool garageDoorMoving = false;
void toggleGarageDoor()
{
  if (garageDoorMoving) return;   // do nothing if we are already moving

  // door not moving. Move it.
  garageDoorMoving = true;
  digitalWrite(GARAGE_DOOR, HIGH); // Turn relay on
  console.println("Moving garage door!");
  garageDoorDelay.attach(GARAGEDOOR_MOVE_TIME, turnOffGarageDoor); // turn it off after 1 second

}

// after time has elasped. stop the move signal and update the topic
void turnOffGarageDoor()
{
  garageDoorMoving = false;
  digitalWrite(GARAGE_DOOR, LOW);
  garageDoorDelay.detach();
  mqtt_client.publish(mqtt_garagedoor_topic, "OFF");
}
