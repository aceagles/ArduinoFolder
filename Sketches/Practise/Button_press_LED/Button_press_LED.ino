int LED = 12;
int LED2 = 8;
int Button = 10;

void setup() {

  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);
}


void loop() {
  digitalWrite(LED, digitalRead(Button));   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, !digitalRead(Button));
  delay(100);                       // wait for a second
}
