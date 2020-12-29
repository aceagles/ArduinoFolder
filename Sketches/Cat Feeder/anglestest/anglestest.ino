#include<Bounce2.h>
#include<Servo.h>

Servo servo;

Bounce bounce;

int angle = 45, dir = -1;



void setup() {
  // put your setup code here, to run once:
  servo.attach(5);
  servo.write(90);
  pinMode(3, INPUT_PULLUP);
  bounce.attach(3);
  bounce.interval(10);

  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  bounce.update();
  if(bounce.fell()){
    dispense(3);
    Serial.println("dispensing");
  }

  if(Serial.available()){
    int nm = Serial.parseInt();
    angle = nm;
    Serial.println(angle);
  }
}

void dispense(int n){
  for(int i = 0; i < n; i++){
    servo.write(90 + dir* angle);
    delay(1000);
    servo.write(90);
    delay(1000);
    dir = dir*-1;
    Serial.println(dir);
  }
}
