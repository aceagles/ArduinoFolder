#include<ledMatrix.h>
int latchPin = 12;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 13;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 11;           // Pin connected to DS of 74HC595（Pin14）
int LEDPin[] = {2, 3, 4, 5, 6, 7, 8, 9}; 
const int smilingFace[] = {
  0x1C, 0x22, 0x51, 0x45, 0x45, 0x51, 0x22, 0x1C
};
const int one[] = {
0x00, 0x00, 0x21, 0x7F, 0x01, 0x00, 0x00, 0x00
};
const int space[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const int tester[] {
   0x1C, 0x22, 0x51, 0x45, 0x45, 0x51, 0x22, 0x1C,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x21, 0x7F, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
   
ledMatrix matrix(latchPin, clockPin, dataPin, LEDPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(sizeof(tester));
  matrix.staticImage(smilingFace);
  matrix.staticImage(one);

  matrix.scrollingImage(tester);
  delay(2000);
}
