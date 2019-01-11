#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <softPwm.h>

typedef enum pwmType {
  HARDWARE,
  SOFTWARE
} pwm;

typedef enum pinControl {
  TWO,
  THREE
} pins;

class Motor {
private:
  short enableDisable, direction1, direction2;
  pwm type;
  pins pinNum;

protected:
  void setupSoftPwm();
  void setupHardPwm();
  void hardForward(int strength);
  void hardBackward(int strength);
  void hardStop();
  void softForward(int strength);
  void softBackward(int strength);
  void softStop();
public:
  Motor(int pinEnable, int pinDirection1, int pinDirection2, pwm type);
  Motor(int pinDirection1, int pinDirection2);
  void forward(int strength);
  void backward(int strength);
  void stop();
};

#endif
