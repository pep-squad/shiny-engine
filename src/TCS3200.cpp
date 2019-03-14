#include "TCS3200.h"
// Public
TCS3200::TCS3200(int s3, int s4, int out) : s3(s3), s4(s4), out(out) {
  wiringPiSetup();
  pinMode(s3, OUTPUT);
  pinMode(s4, OUTPUT);
  pinMode(out, INPUT);
}

Colour TCS3200::scan() {
  RGB rgb;
  rgb.red = getCount(LOW, LOW); // Red
  rgb.green = getCount(HIGH, HIGH); // Green
  rgb.blue = getCount(LOW, HIGH); // Blue
  Colour colour = getColour(rgb);
  return colour;
}

Colour TCS3200::getColour(RGB rgb) {
  Colour colour = BLACK;
  BoolRGB rgbBool;
  std::cout << rgb.red << ":" << rgb.green << ":" << rgb.blue << std::endl;
  if (rgb.red >= 70) {
    rgbBool.r = true;
  } else {
    rgbBool.r = false;
  }
  if (rgb.green >= 70) {
    rgbBool.g = true;
  } else {
    rgbBool.g = false;
  }
  if (rgb.blue >= 70) {
    rgbBool.b = true;
  } else {
    rgbBool.b = false;
  }
  if (rgbBool.r == yellowBool.r && rgbBool.g == yellowBool.g && rgbBool.b == yellowBool.b) {
    colour = YELLOW;
  } else if (rgbBool.r == greenBool.r && rgbBool.g == greenBool.g && rgbBool.b == greenBool.b) {
    colour = GREEN;
  } else if (rgbBool.r == redBool.r && rgbBool.g == redBool.g && rgbBool.b == redBool.b) {
    colour = RED;
  }



  /*int diff = 20;
  std::cout << rgb.red << ":" << rgb.green << ":" << rgb.blue << std::endl;
  if ((rgb.red >= (trans.red-diff) && rgb.red >= (trans.red+diff)) && (rgb.red >= (trans.green-diff) && rgb.red >= (trans.green+diff)) && rgb.blue > trans.blue) {
    colour = BLACK;
  }else if (rgb.red >= white.red && rgb.green >= white.green && rgb.blue >= white.blue) {
    colour = WHITE;
  } else if (rgb.red >= red.red && rgb.green < red.green && rgb.blue > red.blue) {
    colour = RED;
  } else {
    colour = BLUE;
  }*/
  return colour;
}

int TCS3200::getCount(int modeOne, int modeTwo) {
    digitalWrite(s3, modeOne);
    digitalWrite(s4, modeTwo);
    delay(10);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    float duration = 0;
    int count = 0;
    while (duration < 10) {
      while(digitalRead(out) != 0); // wait for low signal
      while(digitalRead(out) != 1); // wait for low signal
      count++;
      end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    }
  return count;
}
