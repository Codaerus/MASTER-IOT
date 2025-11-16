#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);
char msg[80];
unsigned long t = 0;

// ----------- ROOT CA1  --------------//
static const char AWS_CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF"; 


// ----------- CERTIFICADO DEL DISPOSITIVO --------------//
static const char AWS_DEVICE_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUfFXPjauTdHA5JcmulpgxQjTKn8owDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTExNjEzMzkw
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALVTD7AsupvM0PW5UrsG
2go9DT87piaUvs4hrwVNwknnm/qP1jSb159L4hmkevFUhEYno2GSMniUtW8yclg2
tR5lbZrz5uAMsxyWjTqdyy3be6ZFRhsU59UCi6qkY14GoIAYmpl+PPL4qePq48rx
NBIr8vxJbrXFmAPufDana4LsXM8umRe73h7eMxDQcavhuju108mn3+ssqkii6Bjf
UobCv2Uj3hfPAlg1K9YKpBJjfgPIvNBxsotNGM3ADX8Qsm2W7ZIWCdkA7E59HX1u
fy+gqfQV7ZXB15sNer0cY41FDbpLUz9rM85n9+RnULuV4AzJVfgr2K7tJI8fY34Y
aXsCAwEAAaNgMF4wHwYDVR0jBBgwFoAU0+3znOQVxordVx03etYwRKrL9jkwHQYD
VR0OBBYEFCZ0wanNNIdMV0QeZaJNSN4zhGbPMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAmiGa0hesWkZNuPjV4Gghb7PXi
836u7DcTLjW0/1Ja3wJgqp4egCJGAToBgtEyFRHV+PeLGT6iWR9OP1QoZzdwgWjL
TX1iDyqUfqk7pkKJTGrZ7tcmAJO28Lqvz6tq0s/Jpjvi2w1VyOQJ+jmw7+6p5Woe
PweQNOybWl1BqB/F7CmxfMwuLNO+2Z7tjgEH8nJI4upJqiscmjZSQnZZZ+UXXT+J
2N+4kvGW6Q7ngKtnvqVU73Y64PuLS/v8TFOwQNWMTqlKtA/7pGVEOi5tcebRaA/j
e48DpGxDUR3sarynPrycMEC4FCf9Y3gXwB1EgUZpUs6EM68qitE72NKiQWJf
-----END CERTIFICATE-----
)EOF"; 

//----------------- LLAVE PRIVADA -------------------//
static const char AWS_PRIVATE_KEY[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAtVMPsCy6m8zQ9blSuwbaCj0NPzumJpS+ziGvBU3CSeeb+o/W
NJvXn0viGaR68VSERiejYZIyeJS1bzJyWDa1HmVtmvPm4AyzHJaNOp3LLdt7pkVG
GxTn1QKLqqRjXgaggBiamX488vip4+rjyvE0Eivy/ElutcWYA+58Nqdrguxczy6Z
F7veHt4zENBxq+G6O7XTyaff6yyqSKLoGN9ShsK/ZSPeF88CWDUr1gqkEmN+A8i8
0HGyi00YzcANfxCybZbtkhYJ2QDsTn0dfW5/L6Cp9BXtlcHXmw16vRxjjUUNuktT
P2szzmf35GdQu5XgDMlV+CvYru0kjx9jfhhpewIDAQABAoIBAFwV8dqz7JVhQCH6
2zQWd7dJaCfb4DapxfkjjCIyamBkGof2cn/HQYsp24rQSIu1M3P5f3T27WDljPsK
8J0yM2tbcX2HLpJMgM4c+15wcpY4aCR5+++W/2HjaBDiaYB/9xT26dTYEpYGiSkx
yjvvoVU1MrLDjFEOJ3Sfsj+DaQtUqHmmXZD1NXCiANupSJiJGgbqMbTEwtmGmfaI
vkWJf/UUZdOWPU+6hBP85Ga6hXSeeX9k4+8wmhljkImbzvUkUUrSyDcUI2QM7jBu
0xsodNucpbIgiDiOJPSiDmQl9qpAqNMhI2iO0yNI3LOj5vMu7xbSqM+iOiWiP2fF
L+qsCIECgYEA78+t58t7EGwedee+/TlmrFgLtJBKcp5rX1LuorQK6CfvnH+AHFcq
MkAslzoAzZ4y6LLWD70aLmmW/nT4BKgVURu+FVUXOh9IWoUqktjGWhc97ULTftHl
Wbikq+JAq8vQssl4xOAgoTXaHZT3oINIK45McMD1nnrtXyaEJ+M1NcECgYEAwZCj
EopbYfaIIehjaDQaoLrT+47wMY6Np74geHLacxqHNo6R8aAYHo1am8vhgGA8/yNw
/jDR12LDDGiECUtBBEXuon2GePZtrKSKgGnZn/vQrbvlLz8/UzaKShiYnCIon31w
3F4W8PC/BIaUnHuLZbvcd2xeR37MCXqxR4cyhjsCgYBQSxHoNVocEodUZOautGRr
OgOZTmKO4/oBbawW7/9i4dQ5avtDAfBP6OrFK5ePQPO8s/w2QtOcTKBQzGsqa/jP
MmG/lMD1Jfo8zK+HgWtQSM3kPebAZJjNJKSGJHgQzWowIoL8MCrcr0I7e2A57xrf
8oInkoPqrzABFs1S6aEsQQKBgCdtM5oiLmzsz1vAvwbJnibhBy61Nb2SYM4rDugZ
1N8fJy14qseBqYgHmM4lICCK+cGqUWilPwjVKA+N9l8CQhMBv5K/aLWY1EwXE9MG
PHq2yxGoNQ2Dn31unYpsKiGbX5Tk8ykfzSJ0lyPv2bNqAsHNGyIEI8wCSsMfh56f
Li5jAoGAYoOW7P7PUm/HwFCSQZZL2AuuLBYd5LSYoz2vkw4LQrScIPFcg+iEAFmz
X9S2kuQljvQorl5asbsk+xeZMke2/XvL/bN7TQ0bWhyJDthN7sRdF75dc411auiI
bKNT1wrz8LbsaNwzfzAbHmVmTZPBZhGlxAWxZ93U2mZe8KwRCvk=
-----END RSA PRIVATE KEY-----
)EOF"; 

void setup_aws(){
  espClient.setCACert(AWS_CA_CERT);
  espClient.setCertificate(AWS_DEVICE_CERT);
  espClient.setPrivateKey(AWS_PRIVATE_KEY);
  client.setServer("a1iejirb60zb0-ats.iot.us-east-2.amazonaws.com",8883);
  client.setCallback(callback);
}

void setup() {
  setup_wifi();
  setup_aws();
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
  if(millis()-t>= 60000){
    int id = 1;
    int temp = 30;
    int humi = 60;
    snprintf(msg,80,"{\"ID\":%d, \"TEMP\":%d, \"HUMI\":%d}",id,temp,humi);
    client.publish("canalx",msg);
    t = millis();
  }
}

void setup_wifi(){
  Serial.begin(9600);
  WiFi.begin("MOVISTAR_78A8","NS2ajtQJ7TtDt9m"); //SSID, PASS
  while(WiFi.status() != WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
  Serial.println( WiFi.localIP());
}
