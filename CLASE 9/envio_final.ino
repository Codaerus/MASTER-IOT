#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "SPIFFS.h"

WiFiClientSecure espClient;
PubSubClient client(espClient);
unsigned long int t = 0;
char msg[40];
/**********************/
String Read_rootca;
String Read_cert;
String Read_privatekey;
/**********************/
void setup() {
  setup_wifi();
  /*************** Gestion de archivos *************/
  if(!SPIFFS.begin(true)){
    Serial.println("Fallo al montar SPIFFS");
    return;
    }
  /**************  Root CA *************************/
  File file1 = SPIFFS.open("/AmazonRootCA1.pem","r");
   if(!file1){
    Serial.println("Fallo al abrir el archivo1");
    return;
    }
   while(file1.available()){
      Read_rootca = file1.readString();
      }
   /**************  Cer *************************/
   File file2 = SPIFFS.open("/ebdb16-certificate.pem.crt","r");
   if(!file2){
    Serial.println("Fallo al abrir el archivo2");
    return;
    }
   while(file2.available()){
      Read_cert = file2.readString();
      }
   /**************  Private Key *********************/
   File file3 = SPIFFS.open("/ebdb16-private.pem.key","r");
   if(!file3){
    Serial.println("Fallo al abrir el archivo3");
    return;
    }
   while(file3.available()){
      Read_privatekey = file3.readString();
      }
  /*************************************************/
  char *pRead_rootCA;
  pRead_rootCA = (char*)malloc(sizeof(char)*(Read_rootca.length()+1));
  strcpy(pRead_rootCA,Read_rootca.c_str());

  char *pRead_cert;
  pRead_cert = (char*)malloc(sizeof(char)*(Read_cert.length()+1));
  strcpy(pRead_cert,Read_cert.c_str());
  
  char *pRead_privatekey;
  pRead_privatekey = (char*)malloc(sizeof(char)*(Read_privatekey.length()+1));
  strcpy(pRead_privatekey,Read_privatekey.c_str());
  /*************************************************/
  Serial.println("====================================");
  Serial.write(pRead_rootCA);
  Serial.println();
  Serial.println("=====================================");
  Serial.write(pRead_cert);
  Serial.println();
  Serial.println("=====================================");
  Serial.write(pRead_privatekey);
  Serial.println();
  Serial.println("=====================================");
  /************* Añadimos certificados *************/
  espClient.setCACert(pRead_rootCA); //rootCA
  espClient.setCertificate(pRead_cert); //cert
  espClient.setPrivateKey(pRead_privatekey); //private key
  /*************************************************/
  Serial.println("Autenticando al broker");
  client.setServer("a1iejirb60zb0-ats.iot.us-east-2.amazonaws.com", 8883);
  Serial.println("Ahora MQTT");
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
    int id = 1;
    int temp = 25;
    int humi = 70;
    snprintf(msg,40,"{\"ID\":%d,\"TEMP\":%d,\"HUMI\":%d}",id,temp,humi);  // {"ID":id, "TEMP":temp, "HUMI":humi}
    client.publish("canalx",msg);
    t = millis();
  }
}

void setup_wifi(){
  Serial.begin(115200);
  WiFi.begin("MOVISTAR_78A8","NS2ajtQJ7TtDt9m"); //DHCP
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println(WiFi.localIP());
}
