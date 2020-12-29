#include<Bounce2.h>
#include<Servo.h>
int but = 8, serv = 6;
Servo myservo;
Bounce button = Bounce();
int servDelay = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(but, INPUT_PULLUP);
  button.attach(but);
  button.interval(5);
  myservo.attach(serv);
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.update();
  if(Serial.available()){
    servDelay = Serial.parseInt();
    Serial.print("Delay Set to: ");
    Serial.println(servDelay);
  }
  if(button.fell()){
    Serial.println("Button Press");
    myservo.write(90);
    delay(servDelay);
    myservo.write(0);
  }
  delay(100);
}
