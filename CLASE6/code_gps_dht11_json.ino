#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
/***************** GPS ******************/
#include <TinyGPSPlus.h>
TinyGPSPlus gps;
/**************** DHT11 *****************/
#include <DHT.h>
DHT dht(21, DHT11);
/****************************************/
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long int t = 0;
unsigned long int j = 0;
char msg[30];
char sensor[30];

void setup() {
  setup_wifi();
  client.setServer("3.20.224.30", 1883);
  Serial2.begin(9600,SERIAL_8N1,16,17); //GPS
  dht.begin(); //DHT11
}
void reconnect(){
  if(client.connect("ESP32-1")){
    Serial.println("MQTT OK");
  }
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  if(Serial2.available()){
    char c = Serial2.read();
    if (gps.encode(c)){
      if(millis()-t>10000){
        float lati = gps.location.lat();
        float longi = gps.location.lng();
        snprintf(msg,30,"%.6f,%.6f",lati,longi);
        client.publish("GPS",msg);
        t = millis();
      }
    }

    if(millis()-j>5000){
      StaticJsonDocument <96> doc;
      doc["temp"] = dht.readTemperature(); 
      doc["humi"] = dht.readHumidity();
      String sensor;
      serializeJson(doc,sensor);
      client.publish("DHT11",sensor.c_str());
      j = millis();
    }
  }



}

void setup_wifi(){
  Serial.begin(9600);
  WiFi.begin("MOVISTAR_78A8","NS2ajtQJ7TtDt9m"); //DHCP
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println(WiFi.localIP());
}
