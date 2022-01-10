#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include<Bounce2.h>
#include<LowPower.h>

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire display;


enum states {
  main,
  split,
  lowPower,
  sleepMenu,
  sleepFnctn,
  increaseTSleep,
  intervalMenu,
  intervalFnctn,
  increaseOnInterval,
  increaseOffInterval,
  continuous
};
typedef enum states states;
int selector = 0, lastSelector, tSleep = 0, intOn = 0, intOff = 0, relay = 8;
bool btnLatch = false, intOnBool = false, displayOn = true;
volatile int button;
unsigned long startMillis, activeMillis, but1Millis;
states state = main, lastState = continuous, LPState = continuous;
int loadDiff = 0;

Bounce btn1 = Bounce();
Bounce btn2 = Bounce();



void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  btn1.attach(3);

  btn1.interval(5);

  // put your setup code here, to run once:
  Wire.begin();
  Wire.setClock(400000L);


  attachInterrupt(digitalPinToInterrupt(2) , intPin, FALLING);

#if RST_PIN >= 0
  display.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  display.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  display.setFont(fixed_bold10x15);

  display.clear();
  startMillis = millis();
  but1Millis = activeMillis = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  btn1.update();

  if (btn1.fell()) {
    btnLatch = true;
    button = 2;
    Serial.println("btn2");
  }

 
    displayOn = millis() - activeMillis < 40000;
  stateMachine();

  updateGUI();



}

void stateMachine() {

  states prevState = state;
  switch (state) {
    case main:
      digitalWrite(relay, HIGH);
      if (btnLatch == true) {
        Serial.println(btnLatch);
        btnLatch = false;

        if (button == 2) {
          selector ++;
          if (selector > 2)
            selector = 0;
        }
        if (button == 1) {
          switch (selector) {
            case 0:
              state = sleepMenu;
              break;

            case 1:
              state = intervalMenu;
              break;

            case 2:
              state = continuous;
              break;
          }
          selector = 0;
        }
      }
      if (millis() - activeMillis > 30000)
        state = split;
      break;
    case split:
      
      Serial.println(analogRead(A0));
      int preloadV;
      bool isOn;
      if (digitalRead(relay) == HIGH){
        // Get the pre load voltage
        preloadV = analogRead(A0);
        isOn = false;
      } else {
        isOn = true;
      }
      Serial.println(isOn);
      if ((analogRead(A0)+ loadDiff) > 897) {
        digitalWrite(relay, LOW);
        // If turning on from off then calculate the loadDiff
        if (!isOn) {
          // Wait 1s for the new voltage to settle
          delay (1000);
          // Calculate the voltage drop due to the load
          loadDiff =  preloadV - analogRead(A0);
          Serial.println("Difference: ");
          Serial.println(loadDiff);
        }
      } else {
        digitalWrite(relay, HIGH);
        // reset the loadDiff
        loadDiff = 0;
      }
      displayOn = false;
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      selector = -1;
      if (btnLatch) {
        btnLatch = false;
        selector = 0;
        state = main;
      }

      break;

    case lowPower:

      break;

    case sleepMenu:
      if (btnLatch) {
        btnLatch = false;

        if (button == 2) {
          selector ++;
          if (selector > 2)
            selector = 0;
        }

        else if (button == 1) {
          switch (selector) {
            case 0:
              state = sleepFnctn;
              startMillis = millis();
              break;

            case 1:
              state = increaseTSleep;
              break;

            case 2:
              state = main;
              selector = 0;
              break;
          }

        }

      }
      if (millis() - activeMillis > 20000)
        state = main;
      break;

    case sleepFnctn:
    Serial.println((millis() - startMillis) / (1000));
      if (float((millis() - startMillis)) / float((1000)) < tSleep* 60) {
        digitalWrite(relay, LOW);
      } else {
        digitalWrite(relay, HIGH);
        state = main;
      }
      if (millis() % 30000 == 0) {
        updateStateGUI();
      }
      selector = -1;
      if (btnLatch) {
        btnLatch = false;
        selector = 0;
        state = main;
      }
      break;

    case increaseTSleep:
      tSleep += 5;
      if (tSleep > 100)
        tSleep = 0;
      state = sleepMenu;
      selector = 1;
      break;

    case intervalMenu:
      if (btnLatch) {
        btnLatch = false;

        if (button == 2) {
          selector ++;
          if (selector > 3)
            selector = 0;
        }
        else if (button == 1) {
          switch (selector) {
            case 0:
              state = intervalFnctn;
              intOnBool = true;
              startMillis = millis();
              break;

            case 1:
              state = increaseOnInterval;

              break;

            case 2:
              state = increaseOffInterval;
              break;
            case 3:
              state = main;
              selector = 0;
              break;
          }

        }

      }
      if (millis() - activeMillis > 20000)
        state = main;
      break;

    case intervalFnctn:
      if (intOnBool) {
        if (float((millis() - startMillis)) / float(( 1000)) < 60*intOn) {
          digitalWrite(relay, LOW);
        } else {
          intOnBool = false;
          startMillis = millis();

        }
      } else {
        if (float((millis() - startMillis)) / float(( 1000)) < 60* intOff) {
          digitalWrite(relay, HIGH);
        } else {
          intOnBool = true;
          startMillis = millis();
        }
      }
      if (millis() % 20000 == 0) {
        updateStateGUI();
      }
      selector = -1;
      if (btnLatch) {
        btnLatch = false;
        selector = 0;
        state = main;
      }
      break;

    case increaseOnInterval:
      state = intervalMenu;
      selector = 1;
      intOn += 5;
      if (intOn > 100)
        intOn = 0;

      break;

    case increaseOffInterval:
      state = intervalMenu;
      selector = 2;
      intOff += 5;
      if (intOff > 100)
        intOff = 0;

      break;

    case continuous:
      digitalWrite(relay, LOW);
      selector = -1;
      if (btnLatch) {
        btnLatch = false;
        selector = 0;
        state = main;
        digitalWrite(relay, HIGH);
      }
      break;
  }
  LPState = prevState;


}


void updateStateGUI() {
  switch (state) {
    case main:
      display.clear();
      display.setCursor(0, 0);
      display.println("Sleep");
      display.println("Interval");
      display.println("Continuous");
      break;
    case split:
      display.clear();
      display.setCursor(0, 0);
      display.println("----");
      display.println("SPLIT");
      display.println("----");
      break;

    case lowPower:
      display.clear();
      break;

    case increaseTSleep:
      /*
        display.setCursor(33, 2);
        display.print(tSleep);
        display.print("   ");
      */
      break;
    case sleepMenu:
      if (lastState != increaseTSleep) {
        display.clear();
        display.setCursor(0, 0);
        display.println("Sleep");
        display.print("T: ");
        display.println(tSleep);
        display.println("Back");
      } else {
        display.setCursor(33, 2);
        display.print(tSleep);
        display.print("   ");
      }
      break;

    case sleepFnctn:
      if (lastState != state) {
        display.clear();
        display.setCursor(0, 0);
        display.println("----");
        display.println("SLEEP");
      }
      display.setCursor(0, 4);
      display.print( tSleep - (millis() - startMillis) / (1000 * 60) );
      display.println("  ");
      if (lastState != state)
        display.println("----");
      break;



    case intervalMenu:
      if (lastState != increaseOnInterval || lastState != increaseOffInterval) {
        display.clear();
        display.setCursor(0, 0);
        display.println("Interval");
        display.print("On:  ");
        display.println(intOn);
        display.print("Off: ");
        display.println(intOff);
        display.println("Back");
      } else if (lastState == increaseOnInterval) {
        display.setCursor(55, 2);
        display.print(intOn);
        display.print("   ");
      } else if (lastState == increaseOffInterval) {
        display.setCursor(55, 4);
        display.print(intOff);
        display.print("   ");
      }
      break;

    case intervalFnctn:
      if (lastState != state) {
        display.clear();
        display.setCursor(0, 0);
        display.println("----");
        display.println("Interval");
      }
      display.setCursor(0, 4);
      if (intOnBool) {

        display.print("On - ");
        display.print(intOn - (millis() - startMillis) / (1000 * 60));
      } else {
        display.print("Off - ");
        display.print(intOff  - (millis() - startMillis) / (1000 * 60));
      }
      display.println("   ");
      if (lastState != state)
        display.println("----");
      break;

    case increaseOnInterval:
      /*
        display.setCursor(55, 2);
        display.print(intOn);
        display.print("   ");*/
      break;

    case increaseOffInterval:
      /*
        display.setCursor(55, 4);
        display.print(intOff);
        display.print("   ");
      */
      break;

    case continuous:
      display.clear();
      display.setCursor(0, 0);
      display.println("----");
      display.println("Continous");
      display.println("----");
      break;
  }
  updateSelectorGUI();
}

void updateSelectorGUI() {
  display.setCursor(110, 0);
  display.print(" ");
  display.setCursor(110, 2);
  display.print(" ");
  display.setCursor(110, 4);
  display.print(" ");
  display.setCursor(110, 6);
  display.print(" ");
  switch (selector) {
    case -1:

      break;
    case 0:
      display.setCursor(110, 0);
      display.print("<");
      break;

    case 1:
      display.setCursor(110, 2);
      display.print("<");
      break;

    case 2:
      display.setCursor(110, 4);
      display.print("<");
      break;

    case 3:
      display.setCursor(110, 6);
      display.print("<");

      break;
  }
}
void updateGUI() {
  if (lastState != state) {
    if (displayOn)
      updateStateGUI();
    lastState = state;
    Serial.println(state);

  }
  if (lastSelector != selector) {
    lastSelector = selector;
    if (displayOn)
      updateSelectorGUI();
  }
  if(!displayOn)
    display.clear();
}
void intPin() {
  if(millis() - but1Millis > 200){
  delay(200);
  Serial.println("btn1");
  btnLatch = true;
  button = 1;
  but1Millis = activeMillis = millis();
  }

}
