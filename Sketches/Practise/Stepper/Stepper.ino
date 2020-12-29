#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 32

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 10, 9, 11);

// the previous reading from the analog input
int previous = 0;

void setup()
{
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(200);
}

void loop()
{
  // get the sensor value
  //stepper.step(2048);
  stepper.step(-2048);
  delay(3000);
  stepper.step(2048);
  delay(3000);
}
