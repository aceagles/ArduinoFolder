int IN1 = 10;
int IN2 = 11;
int ACTIVE = IN1, INACTIVE = IN2;
int Button = 5;
int SPEED;

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  SPEED = map(analogRead(A0), 0, 1023, 0, 255);
  Serial.println(SPEED);
  if(digitalRead(Button) == LOW){
    delay(10);
    if(digitalRead(Button)==LOW){
      int temp = ACTIVE;
      ACTIVE = INACTIVE;
      INACTIVE = temp;
      Serial.print(ACTIVE);
    
  }
  }
  Motor(SPEED);
  delay(200);
}

void Motor(int SPD){

    analogWrite(ACTIVE, SPD);
    analogWrite(INACTIVE, 0);

}
