#include <VirtualWire.h>
#include <Bounce2.h>

const int transmit_pin = 9;
char trnsmt = 'S';
int Zero, One;
void setup() {
  // put your setup code here, to run once:
   vw_set_tx_pin(transmit_pin);


 vw_setup(8000);
  pinMode(11, INPUT_PULLUP);
  Serial.begin(38400);

}

void loop() {
  // put your main code here, to run repeatedly:
  Zero = analogRead(A0);
  One = analogRead(A1);
  Serial.println(digitalRead(11));
  Serial.println(Zero);
  Serial.println(One);

  bool zrcn = Zero > 1500 && Zero < 2500;
  bool oncn = One > 1500 && One < 2500;

  if (digitalRead(11) == HIGH) {
    if (Zero < 1500 && One < 1500)
      trnsmt = 'G';
    else if (Zero < 1500 && oncn)
      trnsmt = 'L';
    else if (Zero > 2500 && oncn)
      trnsmt = 'R';
    else if (Zero < 1500 && One > 2500)
      trnsmt = 'H';
    else if (Zero > 2500 && One >2500)
      trnsmt = 'J';
    else if (Zero > 2500 && One < 1500)
      trnsmt = 'I';
    else if (zrcn && One >  2500)
      trnsmt = 'B';
    else if (zrcn && One < 1500)
      trnsmt = 'F';
    else
      trnsmt = 'S';
  } else {
    trnsmt = 'P';
  }
  char trn[1] = {trnsmt};
  Serial.println(trnsmt);
    Serial.println("----");
  vw_send((uint8_t *)trn, 1);
  vw_wait_tx();


  delay(200);
}
