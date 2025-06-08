#define TINY_GSM_MODEM_SIM900
#include <TinyGsmClient.h>

#define SerialMon Serial
#define SerialAT Serial2
TinyGsm modem(SerialAT);

/****************MQTT sobre GPRS*********/
#include <PubSubClient.h>
TinyGsmClient gprsClient(modem);
PubSubClient client(gprsClient);
unsigned long t = 0;
char msg[30];
/****************************************/

char apn[] = "datos.personal.com";
char gprsUser[] = "datos";
char gprsPass[] = "datos";

void setup() {
  setup_gprs();
  client.setServer("18.226.98.151",1883);
  client.setCallback(callback);
}

void callback(const char* topic, byte* payload, unsigned int length)
{
  payload[length] = '\0';
  String val = String((char*)payload);
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
  if(millis()-t>= 30000){
    int temp = 14;
    int humi = 70;
    snprintf(msg,30,"%d,%d",temp,humi);
    client.publish("canalx",msg);
    t = millis();
  }
}

void setup_gprs(){
  SerialMon.begin(9600);
  SerialAT.begin(9600, SERIAL_8N1,16,17);
  delay(2000);
  SerialMon.println("Iniciando modem ...");
  modem.restart(); //Reinicia el modem
  String modemInfo = modem.getModemInfo(); //Version del firmware
  SerialMon.println(modemInfo);

  if(!modem.waitForNetwork()){
    SerialMon.println("Error");
    delay(1000);
    return;
  }
  if(modem.isNetworkConnected()){
    SerialMon.println("Conexión a la Red");
  }

  //GPRS Conexión a Internet
  SerialMon.print("Conectandonos a ");
  SerialMon.println(apn);
  if(!modem.gprsConnect(apn,gprsUser,gprsPass)){
    SerialMon.println("Error de conexión GPRS");
    return;
  }
  if(modem.isGprsConnected()){
    SerialMon.println("Conexión GPRS Exitosa");
    SerialMon.println(modem.getLocalIP());
  }
}
