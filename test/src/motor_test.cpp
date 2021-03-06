#include "motor_test.h"

#include <chrono>
#include <ctime>
#include <fstream>

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


void testMotors() {
  //motor 1 setup
  MotorPins motorPin;
  motorPin.direction1 = 4;
  motorPin.direction2 = 5;
  motorPin.strength = 0;
  motorPin.flag = START;
  Motor motor(motorPin.direction1, motorPin.direction2, 6, 26); //Motor 1

  //Motor 2 direction pins
  MotorPins motorPin2;
  motorPin2.direction1 = 28;
  motorPin2.direction2 = 27;
  motorPin2.strength = 0;
  motorPin2.flag = START;
  Motor motor2(motorPin2.direction1, motorPin2.direction2, 25, 29); //Motor 2

  //Motor 3 direction pins
  MotorPins motorPin3;
  motorPin3.direction1 = 22;
  motorPin3.direction2 = 21;
  motorPin3.strength = 0;
  motorPin3.flag = START;
  Motor motor3(motorPin3.direction1, motorPin3.direction2, 24, 23); //Motor 3

  //Motor 4 direction pins
  MotorPins motorPin4;
  motorPin4.direction1 = 7;
  motorPin4.direction2 = 0;
  motorPin4.strength = 0;
  motorPin4.flag = START;
  Motor motor4(motorPin4.direction1, motorPin4.direction2, 2, 3); //Motor 4

  int oldA[4] = {motor.getEncoderA(), motor2.getEncoderA(), motor3.getEncoderA(), motor4.getEncoderA()};
  int oldB[4] = {motor.getEncoderB(), motor2.getEncoderB(), motor3.getEncoderB(), motor4.getEncoderB()};
  int Bin_Value = 0;
  int newA[4] = {0, 0, 0, 0};
  int newB[4] = {0, 0, 0, 0};
  int encCount[4] = {0, 0, 0, 0};
  int encError[4] = {0, 0, 0, 0};
  float rpm[4] = {0, 0, 0, 0};
  int i = 0;
  float rpm_time = 0;
  int scale = 30*7*4;

  int MotorPower = 30;
  motor.forward(MotorPower);
  motor2.forward(30);
  motor3.forward(30);
  motor4.forward(30);

  auto start = std::chrono::high_resolution_clock::now();
  auto finish = start;
  auto deltaT = start;

  while (std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() < 5000) {

    newA[0] = motor.getEncoderA();
    newB[0] = motor.getEncoderB();
    newA[1] = motor2.getEncoderA();
    newB[1] = motor2.getEncoderB();
    newA[2] = motor3.getEncoderA();
    newB[2] = motor3.getEncoderB();
    newA[3] = motor4.getEncoderA();
    newB[3] = motor4.getEncoderB();
    //binarize encoder old and new values
    for (i = 0; i < 4; i++) {
      Bin_Value = oldA[i]*8+oldB[i]*4+newA[i]*2+newB[i];
      IncrementEncoders(Bin_Value, encCount[i], encError[i]);
    }

    oldA[0] = newA[0];
    oldB[0] = newB[0];
    oldA[1] = newA[1];
    oldB[1] = newB[1];
    oldA[2] = newA[2];
    oldB[2] = newB[2];
    oldA[3] = newA[3];
    oldB[3] = newB[3];


    rpm_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - deltaT).count();
    if (rpm_time > 100) {
      for (i = 0; i < 4; i++) {
        rpm[i] = encCount[i]/(rpm_time/1000 * scale)*60;
        encCount[i] = 0;
      }
      if (rpm[0] > 50) {
        MotorPower--;
        motor.forward(MotorPower);
      }
      else if(rpm[0] < 50) {
        MotorPower++;
        motor.forward(MotorPower);
      }
     deltaT = std::chrono::high_resolution_clock::now();
    }



    finish = std::chrono::high_resolution_clock::now();
  }

  for (i = 0; i < 4; i++) {
    std::cout<<"Encoder: "<< i+1 <<std::endl;
    std::cout<<encCount[i]<<std::endl;
    std::cout<<encError[i]<<std::endl;
    printf("%f\n", rpm[i]);
  }

  // delay(2000);
  motor.stop();
  motor2.stop();
  motor3.stop();
  motor4.stop();
  delay(2000);

}
