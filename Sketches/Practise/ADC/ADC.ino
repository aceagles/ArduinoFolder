void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(3));
  //if( digitalRead(3)==LOW){
  int A = map(constrain(analogRead(A0), 370, 600), 370, 600, 0, 255) ;
  Serial.println(A);
  analogWrite(11,A);
  //}
  delay(200);
}
