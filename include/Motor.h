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

typedef enum encoderType {
  ON,
  OFF
} Encoder;

typedef enum pinControl {
  TWO,
  THREE
} pins;

class Motor {
private:
  short enableDisable, direction1, direction2, encoderA, encoderB;
  pwm type;
  pins pinNum;
  Encoder encType;
  int newA, newB, oldA, oldB, count;
  float rpm;
protected:
  void setupSoftPwmEncoder();
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
  Motor(int pinDirection1, int pinDirection2, int encoderA, int encoderB);
  void forward(int strength);
  void backward(int strength);
  void stop();
  void encoderStage(int encVal);
  void setupSoftPwm();

  void setOldA(int val);
  void setOldB(int val);
  void setNewA(int val);
  void setNewB(int val);
  void setCount(int val);
  void setRpm(float val);

  void setDirection1(int pin);
  void setDirection2(int pin);
  void setEncoderA(int pin);
  void setEncoderB(int pin);

  int getEncoderA();
  int getEncoderB();
  int getOldA();
  int getOldB();
  int getNewA();
  int getNewB();
  int getCount();
  float getRpm();
};

#endif
