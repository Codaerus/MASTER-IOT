#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long int t = 0;
char msg[30];
void setup() {
setup_wifi();
client.setServer("3.128.201.171", 1883);
client.setCallback(callback);
}

void callback(const char* topic, byte* payload, unsigned int length){
  payload[length] = '\0';
  String val = String((char*)payload);
  Serial.println(val);
}

void reconnect(){
  if(client.connect("ESP32-1")){
    Serial.println("MQTT OK");
    client.subscribe("canaly");
  }
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  if(millis()-t>10000){
    int temp = 27;
    int humi = 60;
    snprintf(msg,30,"%d,%d",temp,humi);
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
