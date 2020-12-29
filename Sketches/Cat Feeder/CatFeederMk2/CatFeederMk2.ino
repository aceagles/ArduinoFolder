#include<Bounce2.h>
#include<Stepper.h>
#include "RTClib.h"
#include<EEPROM.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool foodLatch = false, tog = false;


int addServ = 0, addHr = 4, addMn = 8;

int but = 6;

Stepper stepper(2048, 8, 10, 9, 11);
Bounce button = Bounce();

int rotAngle = 200, aHour, aMin;

DateTime last;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  stepper.setSpeed(10);
  
  pinMode(but, INPUT_PULLUP);
  button.attach(but);
  button.interval(5);

  rotAngle = eReadInt(addServ);
  
  aHour = EEPROM.read(addHr);
  aMin = EEPROM.read(addMn);
  
  Serial.println(rotAngle);
  Serial.println(aHour);
  Serial.println(aMin);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  button.update();
  DateTime now = rtc.now();

  setData();
  if(button.fell()){
    Serial.println("ButtonPress");
 
    moveServo();
  }
  //Serial.println(aHour);
  if(int(now.hour()) == aHour && int(now.minute())==aMin){
    if(!foodLatch){
    moveServo();
    Serial.println("Food Time!");
    foodLatch = true;
    }
  } else {
    foodLatch = false;
  }
  if(tog){
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  
  Serial.println(now.second());
  }
  delay(100);
}


void setData(){
    if(Serial.available()){
    String s = Serial.readString();
    if(s.indexOf("A") >=0){
      int sep = s.indexOf(":");
      String s1 = s.substring(1,sep);
      String s2 = s.substring(sep+1);
      aHour = s1.toInt();
      aMin = s2.toInt();
      EEPROM.write(addHr, aHour);
      EEPROM.write(addMn, aMin);
      Serial.print("Alarm Set: ");
      Serial.print(aHour);
      Serial.print(":");
      Serial.println(aMin);
      } else if(s.indexOf("D") >=0){
        String s1 = s.substring(1);
        rotAngle = s1.toInt();
        Serial.print("Delay Set to: ");
        Serial.println(rotAngle);
        eWriteInt(addServ, rotAngle);
      }else if(s.indexOf("T") >=0){
        tog = !tog;
      }else if(s.indexOf("S") >=0){
        Serial.println("Setting Tme");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }
      
    }
}

void moveServo(){
  Serial.println("Moving Servo");
  float stp = floor((float(rotAngle)/360)*2048);
  int stpI = int(stp);
  Serial.println(stpI);
  stepper.step(stpI);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

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
