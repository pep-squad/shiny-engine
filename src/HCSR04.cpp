#include "HCSR04.h"

HCSR04::HCSR04(int t, int e) : trig(t), echo(e) {
  wiringPiSetup();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW); 
}

float HCSR04::distance() {
  //TRIG pin must start LOW
  digitalWrite(trig, LOW);
  delay(30);
  //Send trig pulse
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);

  //Wait for echo start
  while(digitalRead(echo) == LOW);

  //Wait for echo end
  long startTime = micros();
  while(digitalRead(echo) == HIGH);
  long travelTime = micros() - startTime;

  //Get distance in cm
  float distance = travelTime / 58;

  return distance;
}
