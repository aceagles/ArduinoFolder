/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER "http://google.com"

#ifndef STASSID
#define STASSID "4GEE_Router_7440_2.4GHz"
#define STAPSK  "DAwey3yu6bpM"
#endif

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    //WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin("http://18.135.107.252/ArduinoInteract.php"); //HTTP
    //http.addHeader("Content-Type", "text/plain");
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String httpData = "password=tedphww&method=get";
    int httpCode = http.POST(httpData);
    //int httpCode = 200;
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        char *nums = strtok (payload, " ");
        int serverNums[];
        i = 0;
        while (nums != NULL){
          serverNums = toInt(nums);
          i++;
          nums = strtok(NULL, " ");
        }
        Serial.println(serverNums[0], serverNums[1]); 
        Serial.println("received payload!!:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
