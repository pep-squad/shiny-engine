#include "TCS3200.h"
// Public
TCS3200::TCS3200(int s3, int s4, int out) : \
    redHigh(RED_HIGH), redLow(RED_LOW), greenHigh(GREEN_HIGH), greenLow(GREEN_LOW), \
    blueHigh(BLUE_HIGH), blueLow(BLUE_LOW), s3(s3), s4(s4), out(out) {
      wiringPiSetup();
      pinMode(s3, OUTPUT);
      pinMode(s4, OUTPUT);
      pinMode(out, INPUT);
    }

RGB TCS3200::scan() {
  RGB rgb;
  rgb.red = getRed();
  rgb.green = getGreen();
  rgb.blue = getBlue();
  return rgb;
}

void TCS3200::calibrate() {
  calibrateRed();
  calibrateGreen();
  calibrateBlue();
}
//Protected
long TCS3200::map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int TCS3200::getRed() {
  digitalWrite(s3, LOW);
  digitalWrite(s4, LOW);
  // simulating pulseIn on arduino
  while(digitalRead(out) != 0); // wait for low signal
  auto start = std::chrono::high_resolution_clock::now();
  while (digitalRead(out) != 1); // wait for high signal
  auto end = std::chrono::high_resolution_clock::now();
  long redFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
  long redColor = map(redFreq, redLow, redHigh, 255, 0);
  if (redColor < 0) {
    redColor = 0;
  } else if (redColor > 255) {
    redColor = 255;
  }
  return redColor;
}

int TCS3200::getGreen() {
  digitalWrite(s3, LOW);
  digitalWrite(s4, HIGH);
  // simulating pulseIn on arduino
  while(digitalRead(out) != 0); // wait for low signal
  auto start = std::chrono::high_resolution_clock::now();
  while (digitalRead(out) != 1); // wait for high signal
  auto end = std::chrono::high_resolution_clock::now();
  long greenFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
  long greenColor = map(greenFreq, greenLow, greenHigh, 255, 0);
  if (greenColor < 0) {
    greenColor = 0;
  } else if (greenColor > 255) {
    greenColor = 255;
  }
  return greenColor;
}

int TCS3200::getBlue() {
  digitalWrite(s3, LOW);
  digitalWrite(s4, HIGH);
  // simulating pulseIn on arduino
  while(digitalRead(out) != 0); // wait for low signal
  auto start = std::chrono::high_resolution_clock::now();
  while (digitalRead(out) != 1); // wait for high signal
  auto end = std::chrono::high_resolution_clock::now();
  long blueFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
  long blueColor = map(blueFreq, 343382, 2031334, 255, 0);
  if (blueColor < 0) {
    blueColor = 0;
  } else if (blueColor > 255) {
    blueColor = 255;
  }
  return blueColor;
}

void TCS3200::calibrateRed() {
  char cal;
  std::cout << "Calibrate Red? [y/n]";
  std::cin >> cal;
  if (cal == 'y') {
    // delay to allow to get ready
    for (size_t i = 3; i > 0; i++) {
      std::cout << "starting calibration in " << i << std::endl;
      sleep(1);
    }
    // begin calibration
    int high = -1;
    int low = 100000000;
    for (size_t i = 0; i < 10000; i++) {
      digitalWrite(s3, LOW);
      digitalWrite(s4, LOW);
      // simulating pulseIn on arduino
      while(digitalRead(out) != 0); // wait for low signal
      auto start = std::chrono::high_resolution_clock::now();
      while (digitalRead(out) != 1); // wait for high signal
      auto end = std::chrono::high_resolution_clock::now();
      long redFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
      if (redFreq > high) {
        high = redFreq;
      }
      if (redFreq < low) {
        low = redFreq;
      }
    }
    setRedHigh(high);
    setRedLow(low);
  }
}

void TCS3200::calibrateGreen() {
  char cal;
  std::cout << "Calibrate Green? [y/n]";
  std::cin >> cal;
  if (cal == 'y') {
    // delay to allow to get ready
    for (size_t i = 3; i > 0; i++) {
      std::cout << "starting calibration in " << i << std::endl;
      sleep(1);
    }
    // begin calibration
    int high = -1;
    int low = 100000000;
    for (size_t i = 0; i < 10000; i++) {
      digitalWrite(s3, HIGH);
      digitalWrite(s4, HIGH);
      // simulating pulseIn on arduino
      while(digitalRead(out) != 0); // wait for low signal
      auto start = std::chrono::high_resolution_clock::now();
      while (digitalRead(out) != 1); // wait for high signal
      auto end = std::chrono::high_resolution_clock::now();
      long greenFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
      if (greenFreq > high) {
        high = greenFreq;
      }
      if (greenFreq < low) {
        low = greenFreq;
      }
    }
    setGreenHigh(high);
    setGreenLow(low);
  }
}

void TCS3200::calibrateBlue() {
  char cal;
  std::cout << "Calibrate Blue? [y/n]";
  std::cin >> cal;
  if (cal == 'y') {
    // delay to allow to get ready
    for (size_t i = 3; i > 0; i++) {
      std::cout << "starting calibration in " << i << std::endl;
      sleep(1);
    }
    // begin calibration
    int high = -1;
    int low = 100000000;
    for (size_t i = 0; i < 10000; i++) {
      digitalWrite(s3, LOW);
      digitalWrite(s4, HIGH);
      // simulating pulseIn on arduino
      while(digitalRead(out) != 0); // wait for low signal
      auto start = std::chrono::high_resolution_clock::now();
      while (digitalRead(out) != 1); // wait for high signal
      auto end = std::chrono::high_resolution_clock::now();
      long blueFreq = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
      if (blueFreq > high) {
        high = blueFreq;
      }
      if (blueFreq < low) {
        low = blueFreq;
      }
    }
    setBlueHigh(high);
    setBlueLow(low);
  }
}
// Private
void TCS3200::setRedHigh(int high) {
  redHigh = high;
}

void TCS3200::setRedLow(int low) {
  redLow = low;
}

void TCS3200::setGreenHigh(int high) {
  greenHigh = high;
}

void TCS3200::setGreenLow(int low) {
  greenLow = low;
}

void TCS3200::setBlueHigh(int high) {
  blueHigh = high;
}

void TCS3200::setBlueLow(int low) {
  blueLow = low;
}
