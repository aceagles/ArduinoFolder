
#include <NewPing.h>
#define Trig 11
#define Echo 12
#define MAXDIST 200

NewPing sonar(Trig, Echo, MAXDIST);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);

  Serial.println(sonar.ping_cm());
}
