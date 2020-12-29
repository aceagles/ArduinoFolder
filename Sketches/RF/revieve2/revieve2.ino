void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  if(analogRead(A0) > analogRead(A1))
    digitalWrite(4, HIGH);
  else
    digitalWrite(4, LOW);

  delay(400);
}
