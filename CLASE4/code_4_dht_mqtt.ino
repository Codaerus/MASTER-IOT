#include <PubSubClient.h>
#include <WiFi.h>
/********* DHT *********************/
#include <DHT.h>
DHT dht(2,DHT11);
char msg[30];
/***********************************/
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long int t = 0;

void setup() {
  setup_wifi();
  client.setServer("18.189.7.185", 1883);
  /****************  DHT ***************/
  dht.begin();
  /*************************************/
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
  if(millis()-t>10000){
    /************ DHT   *****************/
    int temp = dht.readTemperature();
    int humi = dht.readHumidity();
    snprintf(msg,30,"%d,%d",temp,humi);
    Serial.println(msg);
    /************************************/
    client.publish("canalx",msg);
    t = millis();
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
