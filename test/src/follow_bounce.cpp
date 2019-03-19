#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>
#include <fstream>
#include <mutex>

#include "Motor.h"
#include "TCS3200.h"
#include "HCSR04.h"

#define MINIMUM_DISTANCE  20

typedef struct DesiredRpmValues {
  float motor1, motor2, motor3, motor4;
} DesiredRpm;

typedef struct HCSR04GPIOPin {
  int trig;
  int echo;
} DistancePins;

typedef struct MotorPinLayout {
  int direction1, direction2, encoderA, encoderB;
  int strength, count, rpm, posCount;
} MotorPins;

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

void trajectoryPlan(float &Wz, float Vy, float rad, float &delay_time) {
    float pi = 3.14159;
    float alpha = rad*80*pi/180;
    delay_time = alpha/Vy;
    Wz = pi/(2*delay_time);
    delay_time = delay_time*1000;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double sign(double x) {
  if (x > 0.0) return 1.0;
  if (x < 0.0) return -1.0;
  return x;
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
    if (rpm_time > 15) {
      for (unsigned i = 0; i < 4; i++) {
        float rpm = motors[i].getCount()/(rpm_time/1000 * scale)*60;
        motors[i].setRpm(rpm);
        motorPins[i].posCount += motors[i].getCount();
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

int main(int argc, char const *argv[]) {
  float Vx = 0.0; //[mm/s]
  float Vy = 0.0;  //[mm/s] standard forward velocity for robot
  float Wz = 0.0;  //[rad/s]
  DistancePins ultrasonicPins;
  ultrasonicPins.trig = 9;
  ultrasonicPins.echo = 7;
  HCSR04 ultra (ultrasonicPins.trig, ultrasonicPins.echo);
  TCS3200 rgb(12,13,14);
  std::thread t;
  std::vector<MotorPins> motorPins;
  std::vector<Motor> motors;
  bool end = false;
  //motor1 1 setup
  MotorPins motorPin1;
  motorPin1.direction1 = 4;
  motorPin1.direction2 = 5;
  motorPin1.encoderA = 6;
  motorPin1.encoderB = 26;
  motorPin1.rpm = 0;
  motorPin1.strength = 0;
  motorPins.push_back(motorPin1);
  //Motor 2 direction pins
  MotorPins motorPin2;
  motorPin2.direction1 = 27;
  motorPin2.direction2 = 28;
  motorPin2.encoderA = 29;
  motorPin2.encoderB = 25;
  motorPin2.rpm = 0;
  motorPin2.strength = 0;
  motorPins.push_back(motorPin2);
  //Motor 3 direction pins
  MotorPins motorPin3;
  motorPin3.direction1 = 21;
  motorPin3.direction2 = 22;
  motorPin3.encoderA = 23;
  motorPin3.encoderB = 24;
  motorPin3.rpm = 0;
  motorPin3.strength = 0;
  motorPins.push_back(motorPin3);
  //Motor 4 direction pins
  MotorPins motorPin4;
  motorPin4.direction1 = 1;
  motorPin4.direction2 = 0;
  motorPin4.encoderA = 3;
  motorPin4.encoderB = 2;
  motorPin4.rpm = 0;
  motorPin4.strength = 0;
  /*********************/
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
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = 0;
  }
  /*while(1) {
      float dist = ultra.distance();
      std::cout << dist << std::endl;
      usleep(25);
  }*/
  /*int x = 0;
  while (1) {
    Colour col = rgb.scan();
    switch (col) {
      case RED:
        x++;
	std::cout << "RED " << x << "\n";
        break;
      case GREEN:
        std::cout << "GREEN\n";
        break;
      case WHITE:
	x = 0;
        std::cout << "WHITE\n";
        break;
      case BLACK:
	x = 0;
	std::cout << "BLACK\n";
      default:
        //std::cout << "BLACK\n";
        break;
    }
    usleep(25);
  }*/
  float desired_Vy = 150.0;
  float actual_Vy = desired_Vy;
  for (unsigned int cnt = 0; cnt < 8; cnt++) {
    //Vy = 150.0;
    Colour currColour;
    bool white_flag = true;
    bool green_flag = true;
    bool red_flag = true;
    red_flag = true;
    while (red_flag) {
      float dist = ultra.distance();
      if (dist < MINIMUM_DISTANCE) {
          // decrease the speed if getting closer
          for (unsigned i = 0; i < desired_rpm.size(); i++) {
            if (desired_Vy > 0) {
              if ((actual_Vy-2 > 0)) {
                actual_Vy -= 2;
              } else {
                actual_Vy = 0;
              }
            } else if (desired_Vy < 0) {
              if ((actual_Vy+2) < 0) {
                actual_Vy += 2;
              } else {
                actual_Vy = 0;
              }
            }
          }
      } else {
          actual_Vy = desired_Vy;
      }
      motorSpeed(Wz, Vx, actual_Vy, std::ref(desired_rpm));
      for (unsigned i = 0; i < motors.size(); i++) {
        motorPins[i].rpm = desired_rpm[i];
        motorPins[i].posCount = 0;
      }
      white_flag = true;
      green_flag = true;
      currColour = rgb.scan();
      switch (currColour) {
        case RED:
          red_flag = false;
	  std::cout << "RED\n";
          break;
        case GREEN:
        case BLACK:
          green_flag = false;
          std::cout << "GREEN\n";
          break;
        case WHITE:
          std::cout << "WHITE\n";
          white_flag = false;
          break;
        default:
          break;
      }
      if (!green_flag) {
        Vx = -10.0;
        Wz = 0.03;
        motorSpeed(Wz, Vx, actual_Vy, std::ref(desired_rpm));
        for (unsigned i = 0; i < motors.size(); i++) {
          motorPins[i].rpm = desired_rpm[i];
          motorPins[i].posCount = 0;
        }
      } else if (!white_flag) {
        Vx = 10.0;
        Wz = -0.05;
        motorSpeed(Wz, Vx, actual_Vy, std::ref(desired_rpm));
        for (unsigned i = 0; i < motors.size(); i++) {
          motorPins[i].rpm = desired_rpm[i];
          motorPins[i].posCount = 0;
        }
      } else {
        Vx = 0.0;
        Wz = 0.0;
        motorSpeed(Wz, Vx, actual_Vy, std::ref(desired_rpm));
        for (unsigned i = 0; i < motors.size(); i++) {
          motorPins[i].rpm = desired_rpm[i];
          motorPins[i].posCount = 0;
        }
      }
      usleep(25);
    }
    /******************/
    for (unsigned i = 0; i < 4; i++) {
        motors[i].stop();
    }
    /*****************/
    int CornerRad = 100;
    float delay_time = 0.0;
    Vy = 100;
    Vx = 0.0;
    trajectoryPlan(Wz, Vy, CornerRad, delay_time);
    motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
    for (unsigned i = 0; i < 4; i++) {
        motorPins[i].rpm = desired_rpm[i];
        motorPins[i].posCount = 0;
    }
    //delay(delay_time + 50);
    // delay(delay_time);
    int counter = 840*desired_rpm[0]/60*delay_time/1000 - 80;
    while(motorPins[0].posCount < counter) {
      delay(10);
    }
  }
  end = true;
  delay(1000);
  t.detach();
}
