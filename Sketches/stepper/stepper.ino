#include<Stepper.h>

Stepper mystep(48, 8, 9, 10, 11);


void setup() {
  // put your setup code here, to run once:
  mystep.setSpeed(3);
  Serial.begin(9600);
  Serial.println("READY");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int stp = Serial.parseInt();
    Serial.println(stp);
    mystep.step(stp);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }
}
