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
  int diff = 30;
  // std::cout << rgb.red << ":" << rgb.green << ":" << rgb.blue << std::endl;
  // std::cout << white.red << ":" << white.green << ":" << white.blue << std::endl;
  if ((rgb.red >= (white.red-diff) && rgb.red <= (white.red+diff)) && (rgb.green >= (white.green-diff) && rgb.green <= (white.green+diff)) && (rgb.blue >= (white.blue-diff) && rgb.red <= (white.blue+diff))) {
    colour = WHITE;
  // } else if ((rgb.red >= (red.red-diff) && rgb.red <= (red.red+diff)) && (rgb.green >= (red.green-diff) && rgb.green <= (red.green+diff)) && (rgb.blue >= (red.blue-diff) && rgb.red <= (red.blue+diff))) {
  //   colour = RED;
  // } else if ((rgb.red >= (green.red-diff) && rgb.red <= (green.red+diff)) && (rgb.green >= (green.green-diff) && rgb.green <= (green.green+diff)) && (rgb.blue >= (green.blue-diff) && rgb.red <= (green.blue+diff))) {
  //   colour = GREEN;
  // } else if ((rgb.red >= (blue.red-diff) && rgb.red <= (blue.red+diff)) && (rgb.green >= (blue.green-diff) && rgb.green <= (blue.green+diff)) && (rgb.blue >= (blue.blue-diff) && rgb.red <= (blue.blue+diff))) {
  //   colour = BLUE;
  // }  else if ((rgb.red >= (yellow.red-diff) && rgb.red <= (yellow.red+diff)) && (rgb.green >= (yellow.green-diff) && rgb.green <= (yellow.green+diff)) && (rgb.blue >= (yellow.blue-diff) && rgb.red <= (yellow.blue+diff))) {
  //   colour = YELLOW;
  }
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
