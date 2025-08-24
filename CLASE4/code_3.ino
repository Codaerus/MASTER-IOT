#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
char msg[30];
unsigned long t = 0;
void setup() {
  setup_wifi();
  client.setServer("18.189.7.185",1883);
  client.setCallback (callback);
}

void callback(const char *topic, byte* payload, unsigned int length){
  payload[length] = '\0';
  String val  = String((char*)payload);
  Serial.println(val);
}

void reconnect(){
  if(client.connect("ESP32")){
    Serial.println("MQTT OK");
    client.subscribe("canaly");
  }
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  int x = 14;
  int y = 28;
  if(millis()-t>=5000){
    snprintf(msg,30,"%d,%d",x,y);
    client.publish("canalx", msg);
    t = millis();
  }
}


void setup_wifi()
{
  Serial.begin(9600);
  WiFi.begin("MOVISTAR_78A8","NS2ajtQJ7TtDt9m");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
