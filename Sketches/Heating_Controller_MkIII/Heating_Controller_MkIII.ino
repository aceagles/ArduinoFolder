#include <ESP8266HTTPClient.h>
#include <WiFi.h>
#include<Servo.h>

const char* ssid     = "4GEE_Router_7440_2.4GHz";
const char* password = "DAwey3yu6bpM";
Servo myservo;
int threshold = 60;

const char* serverName = "http://pentrefarm.ga/ArduinoInteract.php";
int led = 5;
boolean ledison = false;

void handleLED();
void handleRoot();

void setup() {
    myservo.attach(5);
  // Initialize the output variables as outputs

  myservo.write(90);

  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

    Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');


  //wifiMulti.addAP("BTHub4-QXJX", "ac4d8b348c");   // add Wi-Fi networks you want to connect to


  Serial.println("Connecting ...");
  int i = 0;
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer




}

void loop() {
  // put your main code here, to run repeatedly:
      HttpClient http();
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "method=get";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      

     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String rslt = http.getString();
      Serial.println(rslt);
        
      // Free resources
      http.end();
}

void handleLED(){
  Serial.println("handleLED");
  //toggleLed();
  PressButton();
  server.sendHeader("Location", "/");
  server.send(303);
  
}
