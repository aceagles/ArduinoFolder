#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 
#include<Servo.h>

const char* ssid     = "4GEE_Router_7440_2.4GHz";
const char* password = "DAwey3yu6bpM";
Servo myservo;
int threshold = 60;
String html1 = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}.button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}.button2 {background-color: #77878A;}</style></head><body><h1> Home Heating </h1>Heating is ";

String html2 = "<br> <p><form action=\"/TOG\" method=\"POST\"><input type=\"submit\" value=\"Toggle Heating\"></form></p></body></html>";
//ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'z

ESP8266WebServer server(80);

int led = 4;
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

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/TOG", HTTP_POST, handleLED);  // Call the 'handleLED' function when a POST request is made to URI "/LED"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();  
}

void handleLED(){
  Serial.println("handleLED");
  //toggleLed();
  PressButton();
  server.sendHeader("Location", "/");
  server.send(303);
  
}

void handleRoot(){
  Serial.println("handleRoot");
  String isOn = "Off";
  if(analogRead(A0) > threshold)
    isOn = "On";
 
  String full = html1 + isOn + html2;
  server.send(200, "text/html", full);
}
