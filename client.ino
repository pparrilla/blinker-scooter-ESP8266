#include <ESP8266WiFi.h>


#define btn1 5
#define btn2 4


const char *ssid = "ssid";
const char *password = "password";


int sensor_value1 = 0;        
int sensor_value2 = 0;        

void toggleBtn() {
  if(digitalRead(btn1) == LOW) sensor_value1 = 1;
  if(digitalRead(btn2) == LOW) sensor_value2 = 1;
  
  if(digitalRead(btn1) == HIGH) sensor_value1 = 0;
  if(digitalRead(btn2) == HIGH) sensor_value2 = 0;
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);

              
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}

void loop() {
  
  toggleBtn();

  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("Connected");

  String url = "/data/";
  url += "?sensor_reading=";
  url +=  "{\"sensor1_reading\":\"sensor1_value\",\"sensor2_reading\":\"sensor2_value\"}";

  url.replace("sensor1_value", String(sensor_value1));
  url.replace("sensor2_value", String(sensor_value2));


  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}
