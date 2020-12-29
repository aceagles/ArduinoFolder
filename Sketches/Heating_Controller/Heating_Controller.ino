#include <ESP8266WiFi.h>
#include <Servo.h>
// Replace with your network credentials
const char* ssid     = "BTHub4-QXJX";
const char* password = "ac4d8b348c";

int threshold = 60;

Servo myservo;
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state

int lightThreshold;
long tickFromBoot, tickFromClient;


void setup() {
  Serial.begin(115200);

  myservo.attach(5);
  // Initialize the output variables as outputs

  myservo.write(90);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  tickFromBoot = millis();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");
    tickFromClient = millis();// print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if(millis() - tickFromClient > 30000)
        break;                              //if connected for more than 30 seconds disconnect client. stops phones from locking thread
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            Serial.println(header);

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /press") >= 0) {
              PressButton();
              Serial.println("Press Button");
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body>");

            // Enter WebPage Here

            client.println("<h1> Home Heating </h1>");

            client.println("Heating is ");
            if(analogRead(A0) > threshold)
              client.println("on.");
            else
              client.println("off.");
            client.println("<br>");
            client.println("<p><a href=\"/press\"><button class=\"button\">press</button></a></p>");

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  //ESP.LightSleep(2e6);

  //Restart every 6 hours or so to prevent software hangs. Not great but what ya gonna do
  if (millis()-tickFromBoot > 21600000 || millis()-tickFromBoot < 0) {
    ESP.restart();
  }
}


void PressButton() {
  if (analogRead(A0) > threshold) {
  //if the heating is on then it will usually take 4 button presses to turn it off, check that it has been off twice before stopping
  bool last = true;
  for (int i = 0; i < 4; i++) {
      myservo.write(0);
      delay(500);
      myservo.write(90);
      delay(500);
      
      if(analogRead(A0) < threshold && !last)
        break; 
      last = analogRead(A0) > threshold;
    }

  } else {
    for (int i = 0; i < 2; i++){
    myservo.write(0);
    delay(500);
    myservo.write(90);
    delay(900);
    if(analogRead(A0) > threshold)
      break;
    }
    
  }

}
