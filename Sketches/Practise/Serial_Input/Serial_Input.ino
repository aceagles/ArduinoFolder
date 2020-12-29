int inChar;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Arduino Ready");
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    inChar = Serial.parseInt();
    Serial.print("Data: ");
    Serial.println(inChar);
    analogWrite(9, constrain(inChar, 0, 255));
    Blink();
  }
}

void Blink(){
  for(int i=0; i<5; i++){
  digitalWrite(11, HIGH);
  delay(50);
  digitalWrite(11, LOW);
  delay(50);
  }
}
