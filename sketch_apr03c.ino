/*
  ESP8266 mDNS responder sample

  This is an example of an HTTP server that is accessible
  via http://esp8266.local URL thanks to mDNS responder.

  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch to the ESP8266 board
  - Install host software:
    - For Linux, install Avahi (http://avahi.org/).
    - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
    - For Mac OSX and iOS support is built in through Bonjour already.
  - Point your browser to http://esp8266.local, you should see a response.

*/


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "WiFiManager.h" 



const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";

const char* APSSID = "SpeedAp-";
const char* APPASS = "password";

// TCP server at port 80 will respond to HTTP requests
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;





void setup(void) {
  Serial.begin(115200);
  
  // Connect to WiFi network
  WiFiManager wifiManager;
  String APName = APSSID+String(ESP.getChipId());
   if(!wifiManager.autoConnect(APName.c_str(),APPASS )) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        // reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
    }
/*  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
*/
  
 
  
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

 

  httpUpdater.setup(&server, update_path, update_username, update_password);
  
  // Start TCP (HTTP) server
  server.begin();
  Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void) {

  MDNS.update();
  delay(1);
  server.handleClient();
  main_program();
  delay(1);
}

void main_program(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
 }
