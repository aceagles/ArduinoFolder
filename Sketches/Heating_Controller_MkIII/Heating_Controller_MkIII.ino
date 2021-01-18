#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<Servo.h>

#ifndef STASSID
#define STASSID "4GEE_Router_7440_2.4GHz"
#define STAPSK  "DAwey3yu6bpM"
#endif

Servo myservo;
int threshold = 60;
int isOn;
int led = 4;
boolean ledison = false;


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
  myservo.attach(5);
  myservo.write(90);
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection

  activateHeating();
  delay(20000);
}
