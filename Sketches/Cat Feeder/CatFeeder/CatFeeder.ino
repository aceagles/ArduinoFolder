#include<Bounce2.h>
#include<Servo.h>
#include "RTClib.h"
#include<EEPROM.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool foodLatch = false, tog = false;
int addServ = 0, addHr = 4, addMn = 8;
int but = 8, serv = 6;
Servo myservo;
Bounce button = Bounce();
int servDelay = 200, aHour, aMin;

DateTime last;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(but, INPUT_PULLUP);
  button.attach(but);
  button.interval(5);
  myservo.attach(serv);
  myservo.write(180);
  servDelay = eReadInt(addServ);
  aHour = EEPROM.read(addHr);
  aMin = EEPROM.read(addMn);
  Serial.println(servDelay);
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
        servDelay = s1.toInt();
        Serial.print("Delay Set to: ");
        Serial.println(servDelay);
        eWriteInt(addServ, servDelay);
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
    myservo.write(70);
    //delay(servDelay);
    long shake = millis();
    while (millis() - shake < servDelay){
      myservo.write(100);
      delay(100);
      myservo.write(70);
      delay(100);
    }
    myservo.write(180);
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
