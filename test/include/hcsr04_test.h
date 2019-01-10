#ifndef __HCSR04_TEST_H__
#define __HCSR04_TEST_H__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "HCSR04.h"

typedef struct HCSR04GPIOPin {
  int trig;
  int echo;
} GPIO;

void *hcsr04Scan(int threadid, std::vector<GPIO> pins, std::vector<std::vector<float>> &dist);
void testHCSR04();

#endif
