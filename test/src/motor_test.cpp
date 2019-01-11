#include "motor_test.h"

void setupSoftMotor() {
  Motor motor (27, 28);
  printf("soft forward\n");
  motor.forward(100);
  delay(1000);
  motor.forward(90);
  delay(1000);
  motor.forward(80);
  delay(1000);
  motor.forward(70);
  delay(1000);
  motor.forward(60);
  delay(1000);
  motor.forward(50);
  delay(1000);
  motor.forward(40);
  delay(1000);
  motor.forward(30);
  delay(1000);
  motor.forward(20);
  delay(1000);
  motor.forward(10);
  delay(1000);

  printf("stop\n");
  // motor.forward(0);
  motor.stop();
  delay(1000);

  printf("backwards\n");
  motor.backward(10);
  delay(1000);
  motor.backward(20);
  delay(1000);
  motor.backward(30);
  delay(1000);
  motor.backward(40);
  delay(1000);
  motor.backward(50);
  delay(1000);
  motor.backward(60);
  delay(1000);
  motor.backward(70);
  delay(1000);
  motor.backward(80);
  delay(1000);
  motor.backward(90);
  delay(1000);
  motor.backward(100);
  delay(1000);

  printf("done\n");
  // motor.forward(0);
  motor.stop();
  delay(1000);
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
  // setupHardMotor();
  setupSoftMotor();
}
