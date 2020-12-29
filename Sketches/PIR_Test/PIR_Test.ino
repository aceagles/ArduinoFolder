void setup() {
  // put your setup code here, to run once:
  pinMode(4, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 delay(200);
 Serial.println(digitalRead(4)); 
}
