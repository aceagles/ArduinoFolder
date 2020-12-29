#include<Servo.h>
#include<LowPower.h>

int servoStart = 0, servoStop = 180, servoPin = 6, PIRPin = 2, startDelay = 70000, 
fireDelay = 550, intDelay = 3000, wakeMillis = millis(), hasFired = 0;

boolean fire = false;

Servo servo;

void setup() {
  // Initialise Serial
  Serial.begin(9600);
  Serial.println("startup");

  //Init Servo
  servo.attach(servoPin);
  servo.write(servoStart);

  // Set pin for motion sensor
  pinMode(PIRPin, INPUT);

  //delay while the motion sensor gets warmed up. It miss fires when warming up.
  delay(startDelay);
  Serial.println("Ready...");

  //attach interrupt once sensor has already warmed up so it won't misfire
  attachInterrupt(digitalPinToInterrupt(PIRPin), scareCat, RISING);


}

void loop() {
  // if interrupt has been fired then actuate the servo. Can't be done from within the interurpt because of servos
  if(fire){
    fireAir();
    fire = false;
  }

  // go to sleep once one minute has passed. This is because it takes some time for things to start up again after sleeping
  if (millis - wakeMillis > 60000){
      Serial.println("Sleeping");
  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  }
  delay(100);
}

void scareCat(){
  //ignore the first firing of the interrupt. Seems that one false fire always happens with button interrupts
  if(hasFired >= 2){
  fire = true;
  wakeMillis = millis();
  }
  else{
    hasFired++;
  }

}

void fireAir(){
  //Fire the air for defined amount of time and then don't fire again for defined time
  Serial.println("Firing");
   servo.write(servoStop);
  delay(fireDelay);
  servo.write(servoStart);
  delay(intDelay);
}
