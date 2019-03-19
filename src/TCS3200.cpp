#include "TCS3200.h"
// Public
TCS3200::TCS3200(int s3, int s4, int out) : s3(s3), s4(s4), out(out) {
  whiteColour = 150000;
  redColour = 260000;
  blackColour = 800000;
  prevCount = 0;
  wiringPiSetup();
  pinMode(s3, OUTPUT);
  pinMode(s4, OUTPUT);
  pinMode(out, INPUT);
}

Colour TCS3200::scan() {
  RGB rgb;
  rgb.clear = getCount(HIGH, LOW); // Clear
  //rgb.red = getCount(LOW, LOW); // Red
  //rgb.green = getCount(HIGH, HIGH); // Green
  //rgb.blue = getCount(LOW, HIGH); // Blue
  Colour colour = getColour(rgb);
  return colour;
}

Colour TCS3200::getColour(RGB rgb) {
  Colour colour = BLACK;
  if (rgb.clear > whiteColour && rgb.clear < redColour) {
    colour = GREEN;
  } else if (rgb.clear < whiteColour) {
    colour = WHITE;
  } else if (rgb.clear > blackColour) {
    colour = BLACK;
  } else if (rgb.clear < blackColour) {
    if (prevCount > rgb.clear) {
      colour = RED;
    }
  }
  prevCount = rgb.clear;
  return colour;
}

unsigned long long TCS3200::getCount(int modeOne, int modeTwo) {
    digitalWrite(s3, modeOne);
    digitalWrite(s4, modeTwo);
    delay(10);
    int count = 0;
    unsigned long long total = 0;
    while (count < 10) {
      while(digitalRead(out) != 0); // wait for low signal
      auto start = std::chrono::high_resolution_clock::now();
      while(digitalRead(out) != 1); // wait for low signal
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
      total += duration;
      count += 1;
    }
    std::cout << (total/10) << std::endl;
    return (total/10);
}
