#ifndef __TCS3200_H__
#define __TCS3200_H__

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <chrono>

/****************************************
|   S0    |     S1    |     Frequency   |
|---------------------------------------|
|   LOW   |    LOW    |       Off       |
|   LOW   |    HIGH   |       2%        |
|   HIGH  |    LOW    |       20%       |
|   HIGH  |    HIGH   |       100%      |
*****************************************/

/************************************
|   S2    |     S3    |     Type    |
|-----------------------------------|
|   LOW   |    LOW    |     Red     |
|   LOW   |    HIGH   |     Blue    |
|   HIGH  |    LOW    |     Clear   |
|   HIGH  |    HIGH   |     Green   |
*************************************/


typedef struct rgb {
  int red, green, blue;
} RGB;

class TCS3200 {
private:
  #define  RED_HIGH    1411396
  #define  RED_LOW     96296
  #define  GREEN_HIGH  2682865
  #define  GREEN_LOW   94478
  #define  BLUE_HIGH   2031334
  #define  BLUE_LOW    343382
  int redHigh, redLow, greenHigh, greenLow, blueHigh, blueLow;
  int s3, s4, out;
  void setRedHigh(int high);
  void setRedLow(int low);
  void setGreenHigh(int high);
  void setGreenLow(int low);
  void setBlueHigh(int high);
  void setBlueLow(int low);
protected:
  /* map taken from the Math library in Arduino : https://www.arduino.cc/reference/en/language/functions/math/map/ */
  long map(long x, long in_min, long in_max, long out_min, long out_max);
  int getRed();
  int getGreen();
  int getBlue();
  void calibrateRed();
  void calibrateGreen();
  void calibrateBlue();
public:
  TCS3200(int s3, int s4, int out);
  RGB scan();
  void calibrate();
};

#endif
