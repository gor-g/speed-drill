#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "Ultrasonic.h"



// WIFI connection constants 
const char* ssid = "SpeedDrill";
const char* password = "supasecret";

WiFiUDP udp;
const char* destIP = "192.168.4.1";
const int udpPort = 8989;

// detection constants

const int ULTRASONIC_PIN = 7;

const int DETECTION_TRESHOLD_CM = 50;



Ultrasonic ultrasonic(ULTRASONIC_PIN);


void setup() {
    Serial.begin(115200);
    pinMode(ULTRASONIC_PIN, OUTPUT);

    Serial.print("Connexion to ");
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


    long RangeInCentimeters;
    RangeInCentimeters = ultrasonic.MeasureInCentimeters();
    Serial.print(RangeInCentimeters);//0~400cm
    Serial.println(" cm");

    if (RangeInCentimeters < DETECTION_TRESHOLD_CM)
    {
      udp.beginPacket(destIP, udpPort);
      udp.print(RangeInCentimeters);
      udp.endPacket();

      delay(10000);
    }
}

