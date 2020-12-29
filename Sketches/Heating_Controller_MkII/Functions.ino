void PressButton() {
  if (analogRead(A0) > threshold) {
    //if the heating is on then it will usually take 4 button presses to turn it off, check that it has been off twice before stopping
    bool last = true;
    for (int i = 0; i < 4; i++) {
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(500);

      if (analogRead(A0) < threshold && !last)
        break;
      last = analogRead(A0) > threshold;
    }

  } else {
    for (int i = 0; i < 2; i++) {
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(900);
      if (analogRead(A0) > threshold)
        break;
    }

  }

}

void toggleLed() {
  if (ledison)
    digitalWrite(led, LOW);
  else
    digitalWrite(led, HIGH);
  ledison = !ledison;
}
