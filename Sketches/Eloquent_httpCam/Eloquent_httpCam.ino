#define CAMERA_MODEL_AI_THINKER
#include "WiFi.h"
#include "EloquentVision.h"
#include "ESP32CameraHTTPVideoStreamingServer.h"

using namespace Eloquent::Vision;
using namespace Eloquent::Vision::Camera;

ESP32Camera camera;
HTTPVideoStreamingServer server(81);

/**
 *
 */
void setup() {
    Serial.begin(115200);
    WiFi.softAP("4GEE_Router_7440_2.4GHz", "DAwey3yu6bpM");

    camera.begin(FRAMESIZE_VGA, PIXFORMAT_JPEG);
    server.start();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println(":81' to stream");
}

void loop() {
}
