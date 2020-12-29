void setup() {
  // put your setup code here, to run once:
  pinMode(7, OUTPUT);
  pinMode(5, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(5) == LOW)
    digitalWrite(7, HIGH);
  else
    digitalWrite(7, LOW);
}
