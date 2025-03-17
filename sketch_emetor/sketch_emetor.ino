#include <WiFiS3.h>
#include <WiFiUdp.h>

#define SENSOR_PIN 6

const char* ssid = "SpeedDrill";
const char* password = "supasecret";

WiFiUDP udp;
const char* destIP = "192.168.4.1";
const int udpPort = 8989;

float time;

void setup() {
    Serial.begin(115200);
    pinMode(SENSOR_PIN, OUTPUT);

    Serial.print("Connexion à ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }

    Serial.println("\nConnecté !");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    udp.begin(udpPort);
}

void loop() {
    
    time += .3;

    udp.beginPacket(destIP, udpPort);
    udp.print(time);
    udp.endPacket();

    delay(500);
}

