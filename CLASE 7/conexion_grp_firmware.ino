#define TINY_GSM_MODEM_SIM900
#include <TinyGsmClient.h>

#define SerialMon Serial
#define SerialAT Serial2
TinyGsm modem(SerialAT);

void setup() {
  SerialMon.begin(9600);
  SerialAT.begin(9600, SERIAL_8N1,16,17);
  delay(2000);
  SerialMon.println("Iniciando modem ...");
  modem.restart(); //Reinicia el modem
  String modemInfo = modem.getModemInfo(); //Version del firmware
  Serial.println(modemInfo);
}

void loop() {


}
