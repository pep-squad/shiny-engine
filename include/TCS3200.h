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

class TCS3200 {
private:
  RGB red = { 180, 90, 110 };
  RGB white = { 250, 270,400 };
  RGB green = { 90, 100, 140 };
  RGB yellow = { 500, 390, 300 };
  int s3, s4, out;
protected:
  int getCount(int modeOne, int modeTwo);
  Colour getColour(RGB rgb);
public:
  TCS3200(int s3, int s4, int out);
  Colour scan();
};

#endif
