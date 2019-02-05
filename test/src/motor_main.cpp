#include "Motor.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>

typedef struct MotorPinLayout {
  int direction1, direction2, encoderA, encoderB;
  int strength, count, rpm;
} MotorPins;

void motorThread(bool &end, std::vector<MotorPins> &motorPins, std::vector<Motor> &motors) {
  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].setOldA(motors[i].getEncoderA());
    motors[i].setOldB(motors[i].getEncoderB());
  }

  int Bin_Value = 0;
  float rpm_time = 0;
  int scale = 30*7*4;

  auto start = std::chrono::high_resolution_clock::now();
  auto finish = start;
  auto deltaT = start;

  while (!end) {
    for (unsigned i = 0; i < motors.size(); i++) {
      motors[i].setNewA(motors[i].getEncoderA());
      motors[i].setNewB(motors[i].getEncoderB());
      Bin_Value = motors[i].getOldA()*8+motors[i].getOldB()*4+motors[i].getNewA()*2+motors[i].getNewB();
      motors[i].encoderStage(Bin_Value);
      motors[i].setOldA(motors[i].getNewA());
      motors[i].setOldB(motors[i].getNewB());
    }
    rpm_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - deltaT).count();
    if (rpm_time > 100) {
      for (unsigned i = 0; i < 4; i++) {
        float rpm = motors[i].getCount()/(rpm_time/1000 * scale)*60;
        motors[i].setRpm(rpm);
        motors[i].setCount(0);
        if (rpm > motorPins[i].rpm) {
          motorPins[i].strength--;
        }
        else if(rpm < motorPins[i].rpm) {
          motorPins[i].strength++;
          motors[i].forward(motorPins[i].strength);
        }
        if (motorPins[i].strength > 0 ) {
          motors[i].forward(motorPins[i].strength);
        } else if (motorPins[i].strength < 0) {
          motors[i].backward((motorPins[i].strength*-1));
        } else {
          motors[i].stop();
        }
      }
     deltaT = std::chrono::high_resolution_clock::now();
    }
    finish = std::chrono::high_resolution_clock::now();
  }

  for (unsigned i = 0; i < 4; i++) {
    std::cout<<"Encoder: "<< i+1 <<std::endl;
    std::cout<<motors[i].getCount()<<std::endl;
    // std::cout<<encError[i]<<std::endl;
    printf("%f\n", motors[i].getRpm());
  }

  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].stop();
  }
  delay(100);
}

int main() {
  std::thread t;
  std::vector<MotorPins> motorPins;
  std::vector<Motor> motors;
  bool end = false;
  //motor1 1 setup
  MotorPins motorPin1;
  motorPin1.direction1 = 5;
  motorPin1.direction2 = 4;
  motorPin1.strength = 0;
  motorPin1.encoderA = 26;
  motorPin1.encoderB = 6;
  motorPin1.rpm = -40;
  motorPin1.strength = 0;
  motorPins.push_back(motorPin1);

  //Motor 2 direction pins
  MotorPins motorPin2;
  motorPin2.direction1 = 28;
  motorPin2.direction2 = 27;
  motorPin2.strength = 0;
  motorPin2.encoderA = 25;
  motorPin2.encoderB = 29;
  motorPin2.rpm = -40;
  motorPin2.strength = 0;
  motorPins.push_back(motorPin2);

  //Motor 3 direction pins
  MotorPins motorPin3;
  motorPin3.direction1 = 22;
  motorPin3.direction2 = 21;
  motorPin3.strength = 0;
  motorPin3.encoderA = 24;
  motorPin3.encoderB = 23;
  motorPin3.rpm = -40;
  motorPin3.strength = 0;
  motorPins.push_back(motorPin3);

  //Motor 4 direction pins
  MotorPins motorPin4;
  motorPin4.direction1 = 0;
  motorPin4.direction2 = 7;
  motorPin4.strength = 0;
  motorPin4.encoderA = 3;
  motorPin4.encoderB = 2;
  motorPin4.rpm = -40;
  motorPin4.strength = 0;
  motorPins.push_back(motorPin4);

  motors.reserve(4);
  for (unsigned i = 0; i < motorPins.size(); i++) {
    motors.emplace_back(motorPins[i].direction1, motorPins[i].direction2, motorPins[i].encoderA, motorPins[i].encoderB);
  }

  t = std::thread(motorThread, std::ref(end), std::ref(motorPins), std::ref(motors));
  delay(5000);
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = 0;
  }
  delay(2000);
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = 50;
  }
  delay(5000);
  end = true;
  delay(1000);

  t.detach();
  return 0;
}
