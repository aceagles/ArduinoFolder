#include<Servo.h>
#include<Bounce2.h>

Servo myservo;

Bounce button = Bounce();

void setup() {
  // put your setup code here, to run once:
    myservo.attach(9);
  // Initialize the output variables as outputs
  Serial.begin(115200);
  Serial.println("Starting");
  myservo.write(90);
  pinMode(3, INPUT_PULLUP);
  button.attach(3);
  button.interval(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.update();

  if(button.fell()){
    PressButton();
    Serial.println("Fell");
  }

  
  delay(100);
}

void PressButton() {
      Serial.println("Button Pressed");
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(900);
    

  

}
