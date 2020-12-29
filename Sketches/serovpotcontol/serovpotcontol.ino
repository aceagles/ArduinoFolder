#include<Servo.h>
Servo servo;



void setup() {
  // put your setup code here, to run once:
  servo.attach(6);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int j = analogRead(A0);
  int i = map(j, 0, 1023, 0, 180);
  servo.write(i);
  //analogWrite(11, i);
  Serial.print(i);
  Serial.print(" ");
  Serial.println(j);
  delay(20);
}
