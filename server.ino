#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


#define led0 16               //D0
#define led1 5                //D1
#define led2 4                //D2
#define led3 0                //D3

const char *ssid      = "ssid";
const char *password  = "password";

int sensor_value1 = 0;        
int sensor_value2 = 0;        
String sensor_values;

ESP8266WebServer server(80);

void toggleLeds()
{
  if (sensor_value1 == 0)  {digitalWrite(led0, LOW);digitalWrite(led1, LOW);}
  if (sensor_value2 == 0)  {digitalWrite(led2, LOW);digitalWrite(led3, LOW);}

  if (sensor_value1 == 1)  {
    for (int i = 0; i < 5; i++){
      digitalWrite(led0, HIGH);
      digitalWrite(led1, LOW);
      delay(500);

      digitalWrite(led0, LOW);
      digitalWrite(led1, HIGH);
      delay(500);
        
    }
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
  }
  if (sensor_value2 == 1)  {
    for (int i = 0; i < 5; i++){
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      delay(500);

      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      delay(500);
        
    }
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }
}

void handleSentVar() {

  if (server.hasArg("sensor_reading"))
  {
    sensor_values = server.arg("sensor_reading");
    Serial.println(sensor_values);
  }
  
  StaticJsonDocument<200> doc;
  deserializeJson(doc,sensor_values);

  if(doc["sensor1_reading"] == "0") sensor_value1=0; else sensor_value1=1;
  if(doc["sensor2_reading"] == "0") sensor_value2=0; else sensor_value2=1;

  toggleLeds();

  server.send(200, "text/html", "Data received");
}

void setup() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  toggleLeds();                 
  
  server.on("/data/", HTTP_GET, handleSentVar);
  server.begin();
}

void loop() {
  server.handleClient();
}
