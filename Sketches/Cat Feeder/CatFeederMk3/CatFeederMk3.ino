
#include<Servo.h>
#include "RTClib.h"
#include<EEPROM.h>
#include<LowPower.h>

//Set RTC chip type
RTC_DS3231 rtc;

//Days of week
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Toggle variables
bool foodLatch = false, tog = false, buttLatch = false;

//EEPROM addresses
int addServ = 0, addHr = 4, addMn = 8, addHr2 = 12, addMn2 = 16;

int but = 2, LED = 3;

//toggle for button interrupt
volatile int toggle = 0;

int dir = -1;

//initialise Stepper
Servo servo;
//Alarm and rotation variables
int rotAngle = 200, aHour, aMin, wakeMin, wakeHour, aHour2, aMin2;

DateTime now;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  servo.attach(5);
  servo.write(90);

  //Button to interrupt sleep and manually feed
  pinMode(but, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2) , intPin, FALLING);

  //LED to indicate device is awake
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Read rotation angle from EEPROM using int function
  rotAngle = eReadInt(addServ);
  
  aHour = EEPROM.read(addHr);
  aMin = EEPROM.read(addMn);
  aHour2 = EEPROM.read(addHr2);
  aMin2 = EEPROM.read(addMn2);
  
  
  Serial.println(rotAngle);
  Serial.println(aHour);
  Serial.println(aMin);
  Serial.println(aHour2);
  Serial.println(aMin2);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
  now = rtc.now();

  //set Wake hour, allows device to sleep after 1 min if non use
  wakeMin = now.minute();
  wakeHour = now.hour();

  Serial.println("Ready...");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Update Time
  now  = rtc.now();
  
  //Check for Serial intput
  setData();

  //Check for button press
  if(toggle == 2){

    //Button has been pressed while device is active, move servo
    Serial.println("ButtonPress");
 
    moveServo();

    //toggle = 1 means device is active
    toggle = 1;
  }
  else if (toggle == 1 && !buttLatch){
    //First button press since the device was asleep. set wake time, turn on LED, write to serial
    wakeHour = now.hour();
    wakeMin = now.minute();
    buttLatch = true;
    digitalWrite(LED, HIGH);
    Serial.println("Waking Up");
  }

  
  //Check for alarm
  if((int(now.hour()) == aHour && int(now.minute())==aMin) || (int(now.hour()) == aHour2 && int(now.minute())==aMin2)){
    if(!foodLatch){
    moveServo();
    Serial.println("Food Time!");

    //Only dispense once
    foodLatch = true;
    }
  } 
  else {
    foodLatch = false;
  }

  //output time if requested
  if(tog){
  
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  
  Serial.println(now.second());
  }

  //wait and then sleep for 8s
  delay(30);

  
  if(wakeMin != now.minute() || wakeHour != now.hour()){
  if(toggle == 1){
    //device has been awake, is now going to sleep, tell user, turn off LED, reset button
    Serial.println("Going to Sleep");
    toggle = 0;
    digitalWrite(LED, LOW);
    buttLatch = false;
    delay(400);
  }

  //go to sleep
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  }
}

void intPin(){
  //butto press, increment toggle if less than 2
  Serial.println("Button Press");
  if (toggle < 2)
    toggle++;
  delay(100);
}


void setData(){
  //Read Serial data for setting alarm, food amoutn and time output
  if(Serial.available()){
    String s = Serial.readString();
    
    if(s.indexOf("A") >=0){
      //A is used to set alarm in format 'A 15:35'
      int sep = s.indexOf(":");
      String s1 = s.substring(1,sep);
      String s2 = s.substring(sep+1);
      
      aHour = s1.toInt();
      aMin = s2.toInt();
      
      //write to internal memory so it persists through power cycle
      EEPROM.write(addHr, aHour);
      EEPROM.write(addMn, aMin);
      
      Serial.print("Alarm Set: ");
      Serial.print(aHour);
      Serial.print(":");
      Serial.println(aMin);
      
      } 
        else  if(s.indexOf("B") >=0){
      //A is used to set alarm in format 'A 15:35'
      int sep = s.indexOf(":");
      String s1 = s.substring(1,sep);
      String s2 = s.substring(sep+1);
      
      aHour2 = s1.toInt();
      aMin2 = s2.toInt();
      
      //write to internal memory so it persists through power cycle
      EEPROM.write(addHr2, aHour2);
      EEPROM.write(addMn2, aMin2);
      
      Serial.print("B Alarm Set: ");
      Serial.print(aHour2);
      Serial.print(":");
      Serial.println(aMin2);
      
      } 
      else if(s.indexOf("R") >=0){
        //R used to set rotation angle, in degrees,  in format 'R 260'
        String s1 = s.substring(1);
        rotAngle = s1.toInt();
        
        Serial.print("Rotation Angle Set To: ");
        Serial.println(rotAngle);

        //save rotation angle using eeprom int function
        eWriteInt(addServ, rotAngle);
        
      }else if(s.indexOf("T") >=0){
        //toggle outputting of time 
        tog = !tog;
      }else if(s.indexOf("S") >=0){
        //Set time, this doesn't work
        Serial.println("Setting Tme");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      } else if(s.indexOf("ls")>=0){
        Serial.println("Commands:");
        Serial.println("A ##:##");
        Serial.println("B ##:##");
        Serial.println("R ###");
        Serial.println("T");
        Serial.println("S");
      }
      
    }
}

void moveServo(){
  Serial.println("Moving Servo");

  for(int i = 0; i < 3; i++){
    servo.write(90 + dir* 70);
    delay(1000);
    servo.write(90);
    delay(1000);
    dir = dir*-1;
    Serial.println(dir);
  }
}

/*
 * These functions copied from internet save and read integer values to EEPROM. Not sure if they work properly with negative numbers,
 * will check if it turns out that I need to move stepper anti clockwise too
 */


void eWriteInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  
  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}
 
int eReadInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);
 
  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}
