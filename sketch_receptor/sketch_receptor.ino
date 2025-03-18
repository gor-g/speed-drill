#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "SpeedDrill";
const char* password = "supasecret";

WiFiUDP udp;
const int udpPort = 8989;

int counter = 0;


const int BUZZER_PIN = 7;

const float DISTANCE = 100; // meters
const float TIME = 100*1000; // seconds
const int CHECKPOINTS = 3;
const float INTER_CHECKPOINTS_DURATION = TIME/CHECKPOINTS;


// button stuff
const int ledPin = 3;      // the number of the LED pin, D3
const int buttonPin = 4;    // the number of the pushbutton pin, D4
int buttonState;

unsigned long startTime;
unsigned long elapsedTime;



void setup() {

    

    Serial.begin(115200);
    Serial.println("Estiablishing WiFi...");

    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);


    startTime = millis();
    pinMode(BUZZER_PIN, OUTPUT);

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
    
    int reading = digitalRead(buttonPin);
    if (reading != buttonState) {
          buttonState = reading;
      if (buttonState == LOW) {  //button is pressed
          startTime = millis();
          counter = 0;
      }
    }

    elapsedTime = millis() - startTime;

    int packetSize = udp.parsePacket();
    if (packetSize) {
        int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
        if (len > 0) {
            packetBuffer[len] = '\0';
        }

        long distance = atol(packetBuffer);
        Serial.print("Recieved: ");
        Serial.println(distance);

        // Checks the distance and counts checkpoints
        if (distance > 0 ) {
            counter++;
            Serial.println(elapsedTime);
            if (elapsedTime < counter*INTER_CHECKPOINTS_DURATION)
              good_sound();
            else
              bad_sound();
        }

    }

    delay(50);
}





void bad_sound() {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(250);
    digitalWrite(BUZZER_PIN, LOW);
}



void good_sound(){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(20);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
}
