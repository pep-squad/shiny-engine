#include "motor_test.h"

void setupSoftMotor(int threadid, MotorPins &motorPin) {
  Motor motor(motorPin.direction1, motorPin.direction2);
  int current = motorPin.strength;
  while (motorPin.flag != END) {
    if (motorPin.strength != current) {
      current = motorPin.strength;
      std::cout << "Thread " << threadid << " power " << current << std::endl;
      if (current > 0 ) {
        motor.forward(current);
      } else if (current < 0) {
        motor.backward((current*-1));
      } else {
        motor.stop();
      }
    }
  }
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
  std::vector<std::thread> v;
  std::vector<MotorPins> motorPins;
  MotorPins tempMotor;

  tempMotor.direction1 = 27;
  tempMotor.direction2 = 28;
  tempMotor.strength = 0;
  tempMotor.flag = START;
  motorPins.push_back(tempMotor);
  tempMotor.direction1 = 25;
  tempMotor.direction2 = 24;
  tempMotor.strength = 0;
  tempMotor.flag = START;
  motorPins.push_back(tempMotor);
  tempMotor.direction1 = 23;
  tempMotor.direction2 = 22;
  tempMotor.strength = 0;
  tempMotor.flag = START;
  motorPins.push_back(tempMotor);
  tempMotor.direction1 = 4;
  tempMotor.direction2 = 5;
  tempMotor.strength = 0;
  tempMotor.flag = START;
  motorPins.push_back(tempMotor);

  for (unsigned i = 0; i < motorPins.size(); i++) {
    v.push_back(std::thread(setupSoftMotor, i, std::ref(motorPins[i])));
    delay(500);
  }

  for (int i = 10; i >= 0; i--) {
    for (unsigned j = 0; j < motorPins.size(); j++) {
      motorPins[j].strength = i*10;
    }
    delay(1000);
  }

  for (unsigned i = 0; i <= 10; i++) {
    for (unsigned j = 0; j < motorPins.size(); j++) {
      motorPins[j].strength = i*-10;
    }
    delay(1000);
  }

  for (unsigned i = 0; i < motorPins.size(); i++) {
    motorPins[i].flag = END;
  }
  delay(1000);

  for (unsigned i = 0; i < v.size(); i++) {
    v[i].detach();
  }
}
