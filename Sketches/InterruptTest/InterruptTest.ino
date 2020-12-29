#include <LowPower.h>

#include<avr/sleep.h>
#include<avr/power.h>
#include<avr/interrupt.h>


int interrupt_pin = 2;

volatile int toggle = 0, f_time = 0;

void interruptPin(){
  toggle = 1;

  delay(100);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Ready");
  pinMode(interrupt_pin, INPUT);
  attachInterrupt(0, interruptPin, FALLING);



  //Configure Timer
  TCCR1A = 0x00;

  TCNT1 = 0x0000;

  TCCR1B = 0x05;

  TIMSK1 = 0x01;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(toggle == 1){
    Serial.println("Button");
    toggle =0;
  }
  if(f_time == 1){
    Serial.println("Timer");
    f_time = 0;
  }
  Serial.println("Wakey Wakey");
  delay(100);
  //enterSleep();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  
}
