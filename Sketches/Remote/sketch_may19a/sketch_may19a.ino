#include<VirtualWire.h>

const int transmit_pin = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  vw_set_tx_pin(transmit_pin);

  vw_setup(8000);
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(11));
  delay(500);
}
