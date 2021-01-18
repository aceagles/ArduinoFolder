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

int pollHTTP(String action){
  int toggle;
    if ((WiFi.status() == WL_CONNECTED)) {

    
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin("http://18.135.107.252/ArduinoInteract.php"); //HTTP
    
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    if(action == "get"){
      String httpData = "password=tedphww&method=get";
      int httpCode = http.POST(httpData);   
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          toggle = getValue(payload, ' ', 1).toInt();
          
          Serial.println("received payload!!:\n<<");
          Serial.println(payload);
          Serial.println(isOn);
          Serial.println(toggle);
          Serial.println(">>");
          return toggle;
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    } else if(action == "set"){
      String httpData = "password=tedphww&method=set&isOn=";
       httpData = httpData + isOn + "&toggle=0";
      int httpCode = http.POST(httpData);   
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          toggle = 0;
          
          Serial.println("received payload!!:\n<<");
          Serial.println(payload);

          Serial.println(">>");
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }

    http.end();
  }
  return toggle;
}

void activateHeating(){
  if(pollHTTP("get") == 1)
    PressButton();
  isOn = (analogRead(A0) > threshold ? 1 : 0);
  Serial.println(isOn);
  pollHTTP("set");
}
