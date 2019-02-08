#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <chrono>
#include "TCS3200.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(int argc, char const *argv[]) {
  TCS3200 rgb(12,13,14);
  while (true) {
    Colour colour = rgb.scan();
    switch (colour) {
      case RED:
        std::cout << "RED\n";
        break;
      case GREEN:
        std::cout << "GREEN\n";
        break;
      case BLUE:
        std::cout << "BLUE\n";
        break;
      case YELLOW:
        std::cout << "YELLOW\n";
        break;
      case WHITE:
        std::cout << "WHITE\n";
        break;
      default:
        std::cout << "BLACK\n";
        break;
    }
  }
/*
  int s3 = 12;
  int s4 = 13;
  int out = 14;
  wiringPiSetup();
  pinMode(s3, OUTPUT);
  pinMode(s4, OUTPUT);
  pinMode(out, INPUT);
  auto begin = std::chrono::high_resolution_clock::now();
  auto finish = begin;
  while (std::chrono::duration_cast<std::chrono::milliseconds>(finish-begin).count() < 30000) {
    // RED
    digitalWrite(s3, LOW);
    digitalWrite(s4, LOW);
    delay(10);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    float duration = 0;
    int redCnt = 0;
    while (duration < 10) {
      while(digitalRead(out) != 0); // wait for low signal
      while(digitalRead(out) != 1); // wait for low signal
      redCnt++;
      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }

    // GREEN
    digitalWrite(s3, HIGH);
    digitalWrite(s4, HIGH);
    delay(10);
    start = std::chrono::high_resolution_clock::now();
    end = start;
    duration = 0;
    int greenCnt = 0;
    while (duration < 10) {
      while(digitalRead(out) != 0); // wait for low signal
      while(digitalRead(out) != 1); // wait for low signal
      greenCnt++;
      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }

    // BLUE
    digitalWrite(s3, LOW);
    digitalWrite(s4, HIGH);
    delay(10);
    start = std::chrono::high_resolution_clock::now();
    end = start;
    duration = 0;
    int blueCnt = 0;
    while (duration < 10) {
      while(digitalRead(out) != 0); // wait for low signal
      while(digitalRead(out) != 1); // wait for low signal
      blueCnt++;
      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }


    if ((redCnt >= 90 && redCnt <= 100) && (blueCnt >= 85 && blueCnt <= 95) && (greenCnt >= 85 && greenCnt <= 95)) {
        std::cout << "Yellow\n";
    } else if ((redCnt >= 75 && redCnt <= 85) && (blueCnt >= 70 && blueCnt <= 80) && (greenCnt >= 55 && greenCnt <= 65)) {
      std::cout << "Red\n";
    } else if ((redCnt >= 55 && redCnt <= 65) && (blueCnt >= 77 && blueCnt <= 87) && (greenCnt >= 65 && greenCnt <= 75)) {
      std::cout << "Green\n";
    } else if ((redCnt >= 95 && redCnt <= 105) && (blueCnt >= 150 && blueCnt <= 160) && (greenCnt >= 105 && greenCnt <= 115)) {
      std::cout << "White\n" << std::endl;
    } else if ((redCnt >= 47 && redCnt <= 57) && (blueCnt >= 82 && blueCnt <= 92) && (greenCnt >= 55 && greenCnt <= 65)) {
      std::cout << "Blue\n" << std::endl;
    } else {
        std::cout << "Black\n";
    }
    std::cout << redCnt << ":" << blueCnt << ":" << greenCnt << std::endl;
    // finish = std::chrono::high_resolution_clock::now();
  }
 */
}
