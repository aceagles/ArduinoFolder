#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"


#define leftMotorPin1   3
#define leftMotorPin2   11
#define rightMotorPin1  6
#define rightMotorPin2  5

#define TRIGGER_PIN 9
#define ECHO_PIN 8
#define MAX_DISTANCE 75

#define Kp  20
#define Kd 0.4
#define Ki  180
#define sampleTime  0.005
volatile float targetAngle =  0;

MPU6050 mpu;


int16_t accY, accZ, gyroX;
volatile int motorPower, gyroRate;
volatile float accAngle, gyroAngle, currentAngle, prevAngle=0, error, prevError=0, errorSum=0;
volatile byte count=0;




void setup() {
  Serial.begin(38400);
  Serial.setTimeout(30);
  // set the motor control and PWM pins to output mode
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  // set the status LED to output mode 
  pinMode(13, OUTPUT);
  // initialize the MPU6050 and set offset values
  mpu.initialize();
  mpu.setYAccelOffset(-5694);
  mpu.setZAccelOffset(1119);
  mpu.setXGyroOffset(-59);
  // initialize PID sampling loop
  init_PID();
}

void loop() {

  if(Serial.available()){
    String str = Serial.readStringUntil(";");
    Serial.println(str);
    int i1 = str.indexOf(",");
    int i2 = str.indexOf(",", i1+1);
    int i3 = str.indexOf(",", i2+1);
    String xS = str.substring(0,i1), yS = str.substring(i1+1, i2), potS = str.substring(i2+1, i3);
    int x = xS.toInt();
    int y = yS. toInt();
    int pot = potS.toInt();

    targetAngle = modifiedMap(pot, 0, 1023, -10, 10) + modifiedMap(y, 0, 1023, -3, 3);
    Serial.println(targetAngle);
  }
  // read acceleration and gyroscope values
  accY = mpu.getAccelerationY();
  accZ = mpu.getAccelerationZ();  
  gyroX = mpu.getRotationX();
  // set motor power after constraining it
  motorPower = constrain(motorPower, -255, 255);
  setMotors(motorPower, motorPower);
  //targetAngle = modifiedMap(analogRead(A0), 0, 1023, -30, 30);
}
// The ISR will be called every 5 milliseconds
ISR(TIMER1_COMPA_vect)
{
  // calculate the angle of inclination
  accAngle = atan2(accY, accZ)*RAD_TO_DEG;
  gyroRate = map(gyroX, -32768, 32767, -250, 250);
  gyroAngle = (float)gyroRate*sampleTime;  
  currentAngle = 0.9934*(prevAngle + gyroAngle) + 0.0066*(accAngle);

  error = currentAngle - targetAngle;
  errorSum = errorSum + error;  
  errorSum = constrain(errorSum, -300, 300);
  //calculate output from P, I and D values
  motorPower = Kp*(error) + Ki*(errorSum)*sampleTime + Kd*(currentAngle-prevAngle)/sampleTime;

  prevAngle = currentAngle;
  // toggle the led on pin13 every second
  count++;
  if(count == 200)  {
    count = 0;
    digitalWrite(13, !digitalRead(13));
  }
}

double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 //temp = (int) (4*temp + .5);
 return (double) temp;
}

void setMotors(int leftMotorSpeed, int rightMotorSpeed) {

  if(leftMotorSpeed >= 0) {
    analogWrite(leftMotorPin1, leftMotorSpeed);
    digitalWrite(leftMotorPin2, LOW);
  }
  else {
    analogWrite(leftMotorPin2, -leftMotorSpeed);
    digitalWrite(leftMotorPin1, LOW);
  }
  if(rightMotorSpeed >= 0) {
    analogWrite(rightMotorPin1, rightMotorSpeed);
    digitalWrite(rightMotorPin2, LOW);
  }
  else {
    analogWrite(rightMotorPin2, -rightMotorSpeed);
    digitalWrite(rightMotorPin1, LOW);
  }
}

void init_PID() {  
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B    
  // set compare match register to set sample time 5ms
  OCR1A = 9999;    
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for prescaling by 8
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
}
