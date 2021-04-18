/*
 * ********************************************************************************

 * ********************************************************************************
*/
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <RedGlobals.h>

dConsole console;

void setupConsole()
{
  console.enableSerial(&Serial, true);
  console.enableTelnet(23);
  
}


void handleConsole()
{
  // console
  if (console.check())
  {



    if (strcmp(console.commandString, "?") == 0)
    {
      console.print("\n\n\n[RED]Garage ");
      console.println(VERSION);
      
      console.print(" IP address: ");
      console.println(WiFi.localIP().toString());
      console.println("Available commands are:  door, location room, mqtt server, status, reset (Factory), reboot");
    }
    if ( strcmp(console.commandString, "reset") == 0)
    {
      console.print("Reseting configuration...");
      //reset settings - for testing
      WiFiManager wifiManager;
      wifiManager.resetSettings();
      console.println(" Done.");

    }
    if ( strcmp(console.commandString, "reboot") == 0)
    {
      console.print("Rebooting...");
      delay(200);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
    }
    if ( strcmp(console.commandString, "status") == 0)
    {
      console.println(deviceLocation);
      console.println(myHostName);
      console.printf("MQTT Server %s, port: %s\r\n", mqttServer, mqttPort);
    }


    if (strcmp(console.commandString,"mqtt") == 0)
    {
      strcpy(mqttServer, console.parameterString);
      writeConfigToDisk();
      mqttDisconnect();
      console.print("MQTT server changed to ");
      console.println(mqttServer);

    }
    if (strcmp(console.commandString,"location") == 0)
    {
        strcpy(deviceLocation, console.parameterString);
        writeConfigToDisk();
        console.printf("location changed to %s\r\n",deviceLocation);
        console.println("Change will take effect after next reboot");
    }
    if (strcmp(console.commandString,"door") == 0)
    {
        console.println("Door open");
        toggleGarageDoor();
    }


    console.print("[RED]> ");
  }

}
