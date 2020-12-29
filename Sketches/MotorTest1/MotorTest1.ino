void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int leftMot = map(0, 0, 1023, 0, 255), rightMot = map(500, 0, 1023, 0, 255);
  analogWrite(10, leftMot);
  Serial.println( leftMot);
    Serial.println( rightMot);
    Serial.println("=========");
  analogWrite(6, rightMot);
  delay(200);
}
