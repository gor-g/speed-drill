#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "SpeedDrill";
const char* password = "supasecret";

WiFiUDP udp;
const int udpPort = 8989;

int compteur = 0;
float distanceSeuil = 400;

void setup() {
    Serial.begin(115200);
    Serial.println("Estiablishing WiFi...");

    // creating the wifi access point
    if (WiFi.beginAP(ssid, password)) {
        Serial.println("WiFi is running !");
    } else {
        Serial.println("SpeedDrill WiFi failed !");
        while (1);
    }

    Serial.print("IP : ");
    Serial.println(WiFi.localIP());
    udp.begin(udpPort);
}

void loop() {
    char packetBuffer[255]; // Buffer to tore the recieved data

    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
        if (len > 0) {
            packetBuffer[len] = '\0';
        }

        long distance = atol(packetBuffer);
        Serial.print("Reçu: ");
        Serial.println(distance);

        // Vérification de la distance et comptage des répétitions
        if (distance > 0 && distance < distanceSeuil) {
            compteur++;
            Serial.println(compteur);
        }

    }

    delay(500);
}



