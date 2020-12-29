#include<IRremote.h>

int REC_PIN = 12;
IRrecv irrecv(REC_PIN);
decode_results results;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Ready...");
  irrecv.enableIRIn();
  pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(irrecv.decode(&results)){
  if(results.value != 4294967295)
    Serial.println(results.value, HEX);
    handleButton(results.value);
    irrecv.resume();
    
  }
  delay(100);
}

void handleButton(int result){
  switch(result){
    case 0x20DF10EF:
      digitalWrite(10, !digitalRead(10));
    break;

    case 0xFFA25D:
      digitalWrite(10, !digitalRead(10));
    break;
  }
}
