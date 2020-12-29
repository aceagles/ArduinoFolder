#include<Servo.h>

Servo servo;
int pos = 0;
int servoPin = 13;
int initialPos = 0;


void setup() {
  // put your setup code here, to run once:

  servo.attach(servoPin);
  Serial.begin(9600);
  servo.write(initialPos);
}

void loop() {
  // put your main code here, to run repeatedly:
  //int servopos = floor(map(analogRead(A0),0,1023,76,120));

  servo.write(0);
  delay(2000);
  servo.write(180);
  delay(2000);
    
}
