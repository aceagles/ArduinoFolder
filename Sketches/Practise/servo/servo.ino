#include<Servo.h>

Servo servo;
int pos = 0;
int servoPin = 11;
int initialPos = 90;
int steering = A2, forward = A1, mot1 = 8, mot2=13;

void setup() {
  // put your setup code here, to run once:

  pinMode(steering, INPUT);
  pinMode(forward, INPUT);
  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
  servo.write(initialPos);
}

void loop() {
  // put your main code here, to run repeatedly:
  //int servopos = floor(map(analogRead(A0),0,1023,76,120));
  int joyx = analogRead(steering);
  int joyy = analogRead(forward);
  if(joyx < 100)
    servo.write(130);
  else if (joyx > 900){
    servo.write(60);
  } else {
    servo.write(90);
  }
  Serial.println(joyy);
  if(joyy < 400){
    analogWrite(mot1, map(joyy, 0, 400, 255, 0));
    analogWrite(mot2, 0);
    Serial.println(map(joyy, 0, 400, 255, 0));
  } else if (joyy > 600){
    analogWrite(mot2, map(joyy, 600, 1023, 0, 255));
    analogWrite(mot1, 0);
    Serial.println(map(joyy, 600, 1023, 0, 255));
  } else{
    analogWrite(mot1, 0);
    analogWrite(mot2, 0);
    Serial.println("STOP");
  }
  //servo.write(servopos);
  //Serial.println(servopos);
  delay(50);
    
}
