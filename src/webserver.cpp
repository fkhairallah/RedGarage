#include <ESP8266WebServer.h>
#include <RedGlobals.h>

// Web server instance
ESP8266WebServer server(80);

// Handle root web page
void handleRoot() {
  String html = "<html><head><meta charset=\"UTF-8\"><title>[RED] Garage, Chaleston SC</title>";
  html += "<style>body { font-family: Arial, sans-serif; text-align: center; margin: 50px; }";
  html += "button { font-size: 24px; padding: 20px 40px; margin: 20px; background-color: #4CAF50; color: white; border: none; border-radius: 10px; cursor: pointer; }";
  html += "button:hover { background-color: #45a049; }</style></head><body>";
  html += "<h1>[RED] Garage -- Charleston SC</h1>";
  html += "<p>Indoor Temperature: " + String(currentTemp, 1) + " °F</p>";
  if (currentOutdoorTemp > -100) {
    html += "<p>Outdoor Temperature: " + String(currentOutdoorTemp, 1) + " °F</p>";
  } else {
    html += "<p>Outdoor Temperature: Sensor not available</p>";
  }
  html += "<p>Last updated: " + String(millis() / 60000) + " Minutes since boot</p>";
  html += "<form action='/garagedoor' method='POST'>";
  html += "<button type='submit'>Toggle Garage Door</button>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Handle garage door toggle
void handleGarageDoor() {
  toggleGarageDoor();
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// Start the web server
void startWebServer() {
  server.on("/", handleRoot);
  server.on("/garagedoor", HTTP_POST, handleGarageDoor);
  server.begin();
}

// Handle web server requests
void handleWebServer() {
  server.handleClient();
}