#include<Servo.h>
#include<LiquidCrystal.h>
#include<FlexiTimer2.h>
#include<Bounce2.h>

Bounce button1 = Bounce();
Bounce button2 = Bounce();

LiquidCrystal lcd(12,11,5,4,3,2);
  Servo servo;
 
int servoPin = 13, but1 = 8, but2 = 6, cursorPosIndex = -1;
int hour, minute, Ahour, Aminute, activeTime = 1; 
int cursorPosCol[] = {3, 4, 6, 7, 3, 4, 6, 7};
int cursorPosRow[] = {0, 0, 0, 0, 1, 1, 1, 1};
bool isAlarm= false;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
   
  lcd.begin(16,2);
  FlexiTimer2::set(60000, timerInt);
  FlexiTimer2::start();
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  button1.attach(but1);
  button1.interval(10);
  button2.attach(but2);
  button2.interval(10);
  Serial.begin(9600);
  servo.write(180);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(isAlarm){
    pressKettle();
    isAlarm = false;
  }
  displayLCD();
  button2.update();
  if(button2.fell()){
    increaseVal();
    Serial.write("Button2");
  }

  button1.update();
  if(button1.fell()){
    moveCursor();
    
    Serial.write("Button");
  }
  if (minute >= 60) {     // when minutes is equal to 60, hours plus 1
      minute = minute % 24;
      hour++;
      if (hour >= 24) {     // when hours is equal to 24, hours turn to zero
        hour = hour % 24;
      }
    }
    
    delay(100);
  }

void pressKettle(){
  //insert kettle pressing code here

  servo.write(40);
  delay(1000);
  servo.write(180);
  delay(1000);
  Serial.println("ALARM!!");

  
}

void timerInt(){
  minute++;
  activeTime++;
  if(hour == Ahour && minute == Aminute)
    isAlarm = true;
}

void moveCursor(){
  activeTime = 0;
  if (cursorPosIndex < 7)
    cursorPosIndex++;
  else
    cursorPosIndex = 0;
}

void increaseVal(){
  switch(cursorPosIndex){
    case 0:
    hour += 10;
    break;

    case 1:
    hour ++;
    break;

    case 2:
    minute += 10;
    break;

    case 3:
    minute++;
    break;

    case 4:
    Ahour += 10;
    break;

    case 5:
    Ahour ++;
    break;

    case 6:
    Aminute+=10;
    break;

    case 7:
    Aminute++;
    break;

    default:
    break;
  }
  Ahour = Ahour % 24;
  Aminute = Aminute % 60;
  hour = hour % 24;
  minute = minute % 60;
}
void displayLCD(){
  if (activeTime > 0){
    lcd.noBlink();
    cursorPosIndex = -1;
  }
  lcd.setCursor(0,0);
  lcd.print("T: ");
  if(hour<10)
    lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  if(minute<10)
    lcd.print("0");
  lcd.print(minute);


  lcd.setCursor(0,1);
  lcd.print("A: ");
  if(Ahour<10)
    lcd.print("0");
  lcd.print(Ahour);
  lcd.print(":");
  if(Aminute<10)
    lcd.print("0");
  lcd.print(Aminute);
  if(activeTime ==0){
  lcd.setCursor(cursorPosCol[cursorPosIndex], cursorPosRow[cursorPosIndex]);
  lcd.blink();
  }
}
