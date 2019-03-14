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

typedef enum colourEnum {
  RED,
  BLUE,
  GREEN,
  YELLOW,
  WHITE,
  BLACK
} Colour;

typedef struct rgb {
  int red, green, blue;
} RGB;

typedef struct BooleanRGB {
  bool r, g, b;
} BoolRGB;

class TCS3200 {
private:
  RGB red = { 100, 150, 100 };
  RGB white = { 100, 150, 100 };
  RGB blue = { 40, 40, 80 };
  RGB green = { 60, 110, 100 };
  RGB trans = { 115, 110, 200 };
  BoolRGB redBool = { true, false, false };
  BoolRGB greenBool = { false, true, true };
  BoolRGB yellowBool = { true, true, false };
  BoolRGB blackBool = { false, false, false };
  int s3, s4, out;
protected:
  int getCount(int modeOne, int modeTwo);
  Colour getColour(RGB rgb);
public:
  TCS3200(int s3, int s4, int out);
  Colour scan();
};

#endif
