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

typedef struct DesiredRpmValues {
  float motor1, motor2, motor3, motor4;
} DesiredRpm;

int motorSpeed (float Wz, float Vx, float Vy, std::vector<float> &desired_rpm) {

    float Vb[3] = {Wz, Vx, Vy};
    int idx = 0;
    int flag = 0;


    desired_rpm[0] = (2.9189*Vb[0] - 0.0171*Vb[1] + 0.0171*Vb[2])*9.5495;
    desired_rpm[1] = (2.9189*Vb[0] - 0.0171*Vb[1] - 0.0171*Vb[2])*9.5495;
    desired_rpm[2] = (2.9189*Vb[0] + 0.0171*Vb[1] - 0.0171*Vb[2])*9.5495;
    desired_rpm[3] = (2.9189*Vb[0] + 0.0171*Vb[1] + 0.0171*Vb[2])*9.5495;

	//catch any rpm's that the motor will not be able to output
	for (idx = 0; idx < 4; idx++) {
		if (desired_rpm[idx] > 110) {
			desired_rpm[idx] = 110;
			flag = 1;
	        }
	}

	if (flag == 1) {
		return 0; //check for return in main code for rejected motor setpoint
	}

	return 1;  //all good setpoints
}

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
    if (rpm_time > 10) {
      for (unsigned i = 0; i < 4; i++) {
        float rpm = motors[i].getCount()/(rpm_time/1000 * scale)*60;
        motors[i].setRpm(rpm);
        motors[i].setCount(0);
        if (rpm > motorPins[i].rpm) {
          if ((rpm-5) > motorPins[i].rpm) {
	      motorPins[i].strength -= 2;
	  } else {
              motorPins[i].strength--;
	  }
        }
        else if(rpm < motorPins[i].rpm) {
            if ((rpm+5) < motorPins[i].rpm) {
	        motorPins[i].strength += 2;
	    } else {
                motorPins[i].strength++;
	    }
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
  motorPin1.direction1 = 27;
  motorPin1.direction2 = 28;
  motorPin1.strength = 0;
  motorPin1.encoderA = 29;
  motorPin1.encoderB = 25;
  motorPin1.rpm = 0;
  motorPin1.strength = 0;
  motorPins.push_back(motorPin1);

  //Motor 2 direction pins
  MotorPins motorPin2;
  motorPin2.direction1 = 5;
  motorPin2.direction2 = 4;
  motorPin2.strength = 0;
  motorPin2.encoderA = 26;
  motorPin2.encoderB = 6;
  motorPin2.rpm = 0;
  motorPin2.strength = 0;
  motorPins.push_back(motorPin2);

  //Motor 3 direction pins
  MotorPins motorPin3;
  motorPin3.direction1 = 21;
  motorPin3.direction2 = 22;
  motorPin3.strength = 0;
  motorPin3.encoderA = 23;
  motorPin3.encoderB = 24;
  motorPin3.rpm = 0;
  motorPin3.strength = 0;
  motorPins.push_back(motorPin3);

  //Motor 4 direction pins
  MotorPins motorPin4;
  motorPin4.direction1 = 7;
  motorPin4.direction2 = 0;
  motorPin4.strength = 0;
  motorPin4.encoderA = 2;
  motorPin4.encoderB = 3;
  motorPin4.rpm = 0;
  motorPin4.strength = 0;
  motorPins.push_back(motorPin4);

  motors.reserve(4);
  for (unsigned i = 0; i < motorPins.size(); i++) {
    motors.emplace_back(motorPins[i].direction1, motorPins[i].direction2, motorPins[i].encoderA, motorPins[i].encoderB);
  }

  std::vector<float> desired_rpm;
  desired_rpm.push_back(0.0);
  desired_rpm.push_back(0.0);
  desired_rpm.push_back(0.0);
  desired_rpm.push_back(0.0);
  t = std::thread(motorThread, std::ref(end), std::ref(motorPins), std::ref(motors));

  motorSpeed(0, 0, 200, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(3000);
  motorSpeed(-0.785, 0, 100, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(2100);
  motorSpeed(0, 0, 200, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(3000);
  motorSpeed(-0.785, 0, 100, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(2100);
  motorSpeed(0, 0, 200, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(3000);
  motorSpeed(-0.785, 0, 100, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(2100);
  motorSpeed(0, 0, 200, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(3000);
  motorSpeed(-0.785, 0, 100, std::ref(desired_rpm));
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = desired_rpm[i];
  }
  delay(2100);

  end = true;
  delay(1000);

  t.detach();
  return 0;
}
