
#include<Bounce2.h>
int led1 = 8, led2 = 9;
Bounce button = Bounce();
void setup() {
  // put your setup code here, to run once:

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  button.attach(2);
  button.interval(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  button.update();
  if(button.fell())
    digitalWrite(led1, !digitalRead(led1));
  if(button.rose())
    digitalWrite(led1, !digitalRead(led1));
  if(digitalRead(3)==LOW)
    digitalWrite(led2, !digitalRead(led2));
  delay(50);
}
