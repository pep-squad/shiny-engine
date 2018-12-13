#ifndef __HCSR04_H__
#define __HCSR04_H__

#include <wiringPi.h>

class HCSR04 {
private:
  int trig;
  int echo;
  void setTrigger(int trig);
  void setEcho(int echo);
protected:
public:
  HCSR04(int trig, int echo);
  float distance(); // cm
};

#endif
