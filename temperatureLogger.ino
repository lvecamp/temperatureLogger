#include <ESP8266WiFi.h>
#include <OneWire.h>
#include "DallasTemperature.h"
#include <ArduinoOTA.h>

// Login til WIFI
const char* ssid     = "vestergaard";
const char* password = "dannevirke";
const char* host     = "192.168.1.192";
const int httpPort = 80;

float temp;

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start up the library
  sensors.begin();
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, 10);
  
  ArduinoOTA.begin();
//  pinMode(1, OUTPUT);

}
 
int value = 0;
 
void loop() {
  ArduinoOTA.handle();
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 
  
  temp = sensors.getTempCByIndex(0);

  client.print(String("GET /temp.php?temp=") + String(temp) + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" + 
  "Connection: close\r\n\r\n");
  
  delay(60000);  
/*
  digitalWrite(1, HIGH);
  delay(500);
  digitalWrite(1, LOW);
  delay(500);
*/
}
