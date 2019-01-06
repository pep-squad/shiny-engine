#include "motor_test.h"

void setupSoftMotor() {
  Motor motor (0, 2, 3, SOFTWARE);
  printf("soft forward\n");
  motor.forward(100);
  delay(5000);
  printf("stop\n");
  motor.stop();
  delay(1000);
  printf("backwards\n");
  motor.backward(100);
  delay(5000);
  printf("done\n");
  motor.stop();
}

void setupHardMotor() {
  Motor motor (23, 24, 25, HARDWARE);
  printf("hard forward\n");
  motor.forward(1024);
  delay(5000);
  printf("stop\n");
  motor.stop();
  delay(1000);
  printf("backwards\n");
  motor.backward(1024);
  delay(5000);
  printf("done\n");
  motor.stop();
}

void testMotors() {
  setupHardMotor();
  setupSoftMotor();
}
