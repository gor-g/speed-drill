
#include "Ultrasonic.h"


Ultrasonic ultrasonic(7);
void setup()
{
  pinMode(6, OUTPUT);
  
 Serial.begin(9600);
}


void loop()
{
 long RangeInCentimeters;

 RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
 Serial.print(RangeInCentimeters);//0~400cm
 Serial.println(" cm");

 if(RangeInCentimeters<100){
  beep(RangeInCentimeters);
 }
}



void beep(int distance)
{
  int pause = distance*distance/10;
  digitalWrite(6, HIGH);
  delay(25);
  digitalWrite(6, LOW);
  delay(pause);
}
