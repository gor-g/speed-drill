#include <WiFiS3.h>
#include <WiFiUdp.h>

const char* ssid = "SpeedDrill";
const char* password = "supasecret";

WiFiUDP udp;
const int udpPort = 8989;

int counter = 0;


const int BUZZER_PIN = 7;
const int BUZZER_FREQUENCY = 500;

float DISTANCE = 100; // meters
float TIME = 100*1000; // seconds
int CHECKPOINTS = 3;
float INTER_CHECKPOINTS_DURATION = TIME/CHECKPOINTS;


// button stuff
const int ledPin = 3;      // the number of the LED pin, D3
const int buttonPin = 4;    // the number of the pushbutton pin, D4
int buttonState;

unsigned long startTime;
unsigned long elapsedTime;


// setting constants
const int SETTING_START = -1;


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
    char packetBuffer[255]; // Buffer to store the recieved data
    
    int reading = digitalRead(buttonPin);
    if (reading != buttonState) {
          buttonState = reading;
      if (buttonState == LOW) {  //button is pressed
          countdown();
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

        long distance = processPacketBuffer(packetBuffer);
        Serial.print("Recieved: ");
        Serial.println(distance);

        // Checks the distance and counts checkpoints
        if (distance > 0 ) {
            counter++;
            Serial.println("stats: ");

            Serial.println(counter);
            Serial.println(INTER_CHECKPOINTS_DURATION/1000);
            Serial.println(elapsedTime/1000);
            Serial.println(counter*INTER_CHECKPOINTS_DURATION);


            if (elapsedTime < counter*INTER_CHECKPOINTS_DURATION)
              good_sound();
            else
              bad_sound();
        }

    }

    delay(50);
}





void bad_sound() {
    tone(BUZZER_PIN, BUZZER_FREQUENCY);
    delay(250);
    noTone(BUZZER_PIN);
}



void good_sound(){
    tone(BUZZER_PIN, BUZZER_FREQUENCY);
    delay(20);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, BUZZER_FREQUENCY);
    delay(50);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, BUZZER_FREQUENCY);
    delay(50);
    noTone(BUZZER_PIN);
}


// Function to parse packetBuffer and set global variables
int processPacketBuffer(const char (&packetBuffer)[255]) {
    // Convert the char array to a String
    String packetString = String(packetBuffer);
    Serial.print("message : ");
    Serial.println(packetString);
    // Check if the string contains a comma
    if (packetString.indexOf(',') != -1) {
        // Split the string into three parts
        int firstCommaIndex = packetString.indexOf(',');
        int secondCommaIndex = packetString.indexOf(',', firstCommaIndex + 1);

        DISTANCE = packetString.substring(0, firstCommaIndex).toFloat();
        TIME = packetString.substring(firstCommaIndex + 1, secondCommaIndex).toFloat()*1000;
        CHECKPOINTS = packetString.substring(secondCommaIndex + 1).toInt();
        INTER_CHECKPOINTS_DURATION = TIME/CHECKPOINTS;
        // Return -1 to indicate that the string was split
        return -1;
    } else {
        // Parse the string as a positive number
        return packetString.toInt();
    }
}

void countdown() 
{
    for (int i = 0; i < 3; ++i)
    {
        tone(BUZZER_PIN, BUZZER_FREQUENCY + (i * 10));
        delay(500);
        noTone(BUZZER_PIN);
        delay(500);
    }

    tone(BUZZER_PIN, BUZZER_FREQUENCY + 150);
    delay(1000);
    noTone(BUZZER_PIN);
}
