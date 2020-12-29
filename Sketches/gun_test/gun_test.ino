#include<Servo.h>
#include<Bounce2.h>

Servo serv;
int mot = 3, but = 4;
Bounce button = Bounce();

void setup() {
  // put your setup code here, to run once:
  pinMode(mot, OUTPUT);
  pinMode(but, INPUT_PULLUP);
  serv.attach(6);
  serv.write(100);
  button.attach(but);
  button.interval(10);
  Serial.begin(9600);
  digitalWrite(mot, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.update();
  if(button.fell()){
    shoot();
    Serial.println("Button Pressed");
  }
  delay(50);
}

void shoot(){
  digitalWrite(mot, LOW);
  serv.write(40);
  delay(100);
  serv.write(90);
  delay(100);
  serv.write(0);
  delay(200);
  //serv.write(180);
  delay(200);
  serv.write(180);
  delay(300);
  serv.write(60);
  
  delay(300);
  serv.write(100);
  digitalWrite(mot, HIGH);
}
