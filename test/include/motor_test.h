#ifndef __MOTOR_TEST_H__
#define __MOTOR_TEST_H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "Motor.h"

typedef enum endFlag {
  START,
  MID,
  END
} MotorFlag;

typedef struct MotorPinLayout {
  int direction1, direction2, strength;
  MotorFlag flag;
} MotorPins;

void setupSoftMotor(int threadid, MotorPins &motorPin);
void setupHardMotor();
void testMotors();

#endif
