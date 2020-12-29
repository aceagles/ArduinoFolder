#include<Bounce2.h>

#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4
//int x, y, pot1, pot2;

bool but1, but2;
Bounce debouncer1 = Bounce();

// Instantiate another Bounce object
Bounce debouncer2 = Bounce();
long time;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(5); // interval in ms

  // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(5); // interval in ms
  time = millis();
}

void loop() {
  debouncer1.update();
  debouncer2.update();
  if(debouncer1.fell())
    but1 = true;

 if(debouncer2.fell())
  but2 = true;

  if(millis()-time>= map(analogRead(A3), 0, 1023, 0, 500)){
  // put your main code here, to run repeatedly:
  //Serial.print("x:");
  Serial.print(analogRead(A0));
  Serial.print(",");
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.print(analogRead(A2));
  Serial.print(",");
  Serial.print(analogRead(A3));
  
  Serial.print(",");
  Serial.print(but1);
 
  Serial.print(",");
  Serial.print(but2);
    Serial.println(";");
    time = millis();
    but1 = false;
    but2 = false;

    
}


  //delay(500);
}
