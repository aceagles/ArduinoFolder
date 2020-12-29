int Button = 3;
int D = 1;
void setup() {
  // put your setup code here, to run once:
  for(int i=4;i<=13;i++)
    pinMode(i, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for(int i=4;i<=13 && i>= 4;i+=D)
    toggle(i);
  delay(500);

}

void toggle(int i) {
  float j = 100*i/(13-4);
  int k= floor(j);
  analogWrite(i, map(k,0,100,0,255));
  delay(200);

  //digitalWrite(i, !digitalRead(i));
}
