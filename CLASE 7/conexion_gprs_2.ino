#define TINY_GSM_MODEM_SIM900
#include <TinyGsmClient.h>

#define SerialMon Serial
#define SerialAT Serial2
TinyGsm modem(SerialAT);

char apn[] = "datos.personal.com";
char gprsUser[] = "datos";
char gprsPass[] = "datos";

void setup() {
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
  if(!modem.gprsConnect(apn,gprsUer,gprsPass)){
    SerialMon.println("Error de conexión GPRS");
    return;
  }
  if(modem.isGprsConnected()){
    SerialMon.println("Conexión GPRS Exitosa");
    SerialMon.println(modem.getLocalIP());
  }
}

void loop() {


}
