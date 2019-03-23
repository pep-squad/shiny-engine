#include <chrono>
#include <unistd.h>

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
  // delay(30);
  delay(30);
  //Send trig pulse
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);

  //Wait for echo start
  auto start = std::chrono::high_resolution_clock::now();
  auto end = start;
  while(digitalRead(echo) == LOW) {
    // add in a timeout
    end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (diff > 3000000) {
      return 100;
    }
  }

  //Wait for echo end
  long startTime = micros();
  start = std::chrono::high_resolution_clock::now();
  end = start;
  while(digitalRead(echo) == HIGH) {
    // add in a timeout
    end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (diff > 3000000) {
      return 100;
    }
  }
  long travelTime = micros() - startTime;

  //Get distance in cm
  float distance = travelTime / 58;

  return distance;
}
