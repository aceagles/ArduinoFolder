int LED1 = 11;
int LED2 = 9;
int Button = 3;
int LED1V = 0;
int LED2V = 255;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  analogWrite(LED1, LED1V);
  analogWrite(LED2, LED2V);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(Button));
  if(digitalRead(Button)==HIGH){
    LED1V =ChangeLED(LED1, true, LED1V);
    LED2V =ChangeLED(LED2, false, LED2V);
  }
  else {
    LED1V =ChangeLED(LED1, false, LED1V);
    LED2V =ChangeLED(LED2, true, LED2V);
  }
  delay (90);

}

int ChangeLED(int led, bool up, int j){
  
  if (up) {
    
    if (j <= 240)
      j+= 10;
  }
  else{
    if (j >= 20)
      j -= 10;
  }
  analogWrite(led, j);
  Serial.print (led);
  Serial.print (" ");
  Serial.println(j);
  return j;
      
}
