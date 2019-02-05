#include "Motor.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

typedef enum endFlag {
  START,
  MID,
  END
} MotorFlag;

typedef struct MotorPinLayout {
  int direction1, direction2, encoderA, encoderB;
  int strength, count, rpm;
  MotorFlag flag;
} MotorPins;

void setupSoftMotor(int threadid, MotorPins &motorPin) {
  Motor motor1(motorPin.direction1, motorPin.direction2);
  int current = motorPin.strength;
  while (motorPin.flag != END) {
    if (motorPin.strength != current) {
      current = motorPin.strength;
      std::cout << "Thread " << threadid << " power " << current << std::endl;
      if (current > 0 ) {
        motor1.forward(current);
      } else if (current < 0) {
        motor1.backward((current*-1));
      } else {
        motor1.stop();
      }
    }
  }
  motor1.stop();
}

void setupHardMotor() {
  Motor motor1 (23, 24, 25, HARDWARE);
  printf("hard forward\n");
  motor1.forward(1024);
  delay(5000);
  printf("stop\n");
  motor1.stop();
  delay(1000);
  printf("backwards\n");
  motor1.backward(1024);
  delay(5000);
  printf("done\n");
  motor1.stop();
}

void IncrementEncoders(int Encodervalue, int &count, int &errCount) {
  switch (Encodervalue) {
    case 1:
    case 7:
    case 8:
    case 14:
    //forward direciton
      count++;
      break;
    case 2:
    case 4:
    case 11:
    case 13:
    //reverse direction
      count--;
      break;
    case 0:
    case 5:
    case 10:
    case 15:
      //no change
      break;
    case 3:
    case 6:
    case 9:
    case 12:
      //invalid change - what to do??
      errCount++;
  }
}


int main() {
  // std::vector<Motor> motors;
  std::vector<MotorPins> motorPins;
  //motor1 1 setup
  MotorPins motorPin1;
  motorPin1.direction1 = 4;
  motorPin1.direction2 = 5;
  motorPin1.strength = 0;
  motorPin1.encoderA = 6;
  motorPin1.encoderB = 26;
  motorPin1.flag = START;
  motorPins.push_back(motorPin1);

  //Motor 2 direction pins
  MotorPins motorPin2;
  motorPin2.direction1 = 28;
  motorPin2.direction2 = 27;
  motorPin2.strength = 0;
  motorPin2.encoderA = 25;
  motorPin2.encoderB = 29;
  motorPin2.flag = START;
  motorPins.push_back(motorPin2);

  //Motor 3 direction pins
  MotorPins motorPin3;
  motorPin3.direction1 = 22;
  motorPin3.direction2 = 21;
  motorPin3.strength = 0;
  motorPin3.encoderA = 24;
  motorPin3.encoderB = 23;
  motorPin3.flag = START;
  motorPins.push_back(motorPin3);

  //Motor 4 direction pins
  MotorPins motorPin4;
  motorPin4.direction1 = 7;
  motorPin4.direction2 = 0;
  motorPin4.strength = 0;
  motorPin4.encoderA = 2;
  motorPin4.encoderB = 3;
  motorPin4.flag = START;
  motorPins.push_back(motorPin4);

  std::vector<Motor> motors;
  motors.reserve(4);
  for (unsigned i = 0; i < motorPins.size(); i++) {
    motors.emplace_back(motorPins[i].direction1, motorPins[i].direction2, motorPins[i].encoderA, motorPins[i].encoderB);
  }

  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].setOldA(motors[i].getEncoderA());
    motors[i].setOldB(motors[i].getEncoderB());
  }

  int Bin_Value = 0;
  float rpm_time = 0;
  int scale = 30*7*4;
  int MotorPower = 30;

  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].forward(30);
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto finish = start;
  auto deltaT = start;

  while (std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() < 5000) {

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
      }
      if (motors[0].getRpm() > 50) {
        MotorPower--;
        motors[0].forward(MotorPower);
      }
      else if(motors[0].getRpm() < 50) {
        MotorPower++;
        motors[0].forward(MotorPower);
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
  return 0;
}
