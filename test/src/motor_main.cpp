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
  int strength, count, rpm, posCount;
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

void trajectoryPlan(float &Wz, float Vy, float rad, float &delay_time) {
  float pi = 3.14159;
  float alpha = rad*pi/2; //determine cord length of turn
  delay_time = alpha/Vy;  //time required to make turn at constant forward velocity)
  Wz = pi/(2*delay_time);  //constant rotation of 90 degree (pi/2) over time interval calculated
  delay_time = delay_time*1000; //convert to milliseconds for delay function
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
  std::chrono::high_resolution_clock::time_point lastPulse[4] = {start};
  auto difference = 0;
  int oldCount[4] = {0};
  float rpm[4][3];
  float error[4][2];
  float integralError[4] = {0, 0, 0, 0};
  float derivativeError[4] = {0, 0, 0, 0};
  std::ofstream myfile;
   myfile.open("Speed Data.txt");
  float kp, ki, kd;


  for (unsigned i = 0; i < 4; i++) {
    rpm[i][0] = 0;
    rpm[i][1] = 0;
    rpm[i][2] = 0;
    error[i][0] = 0;
    error[i][1] = 0;
    motorPins[i].posCount = 0;
  }

  while (!end) {
    for (unsigned i = 0; i < motors.size(); i++) {
      motors[i].setNewA(motors[i].getEncoderA());
      motors[i].setNewB(motors[i].getEncoderB());
      Bin_Value = motors[i].getOldA()*8+motors[i].getOldB()*4+motors[i].getNewA()*2+motors[i].getNewB();
      oldCount[i] = motors[i].getCount();
      motors[i].encoderStage(Bin_Value);
      motors[i].setOldA(motors[i].getNewA());
      motors[i].setOldB(motors[i].getNewB());
    }
    rpm_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - deltaT).count();
    for (unsigned i = 0; i < 4; i++) {
	    double rpm_avg = 0;
      if (motorPins[i].rpm < 15 && motorPins[i].rpm > -15) {
		      if ((oldCount[i] != motors[i].getCount())) {
			        rpm[i][2] = rpm[i][1];
          		rpm[i][1] = rpm[i][0];
          		//firstTime[i] = false;
          	  difference = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-lastPulse[i]).count();
          		lastPulse[i] = std::chrono::high_resolution_clock::now();
        			if (difference == 0) {
        				rpm[i][0] = 0;
        			} else {
        				rpm[i][0] = (double)motors[i].getCount() * 1.0 / ((double)difference / 1000000000.0 * 840.0 / 60.0 );
        			}
			        rpm_avg = (rpm[i][0]+rpm[i][1]+rpm[i][2])/3;
          		motors[i].setRpm(rpm_avg);
          		motorPins[i].posCount = motorPins[i].posCount + motors[i].getCount();
          		motors[i].setCount(0);
	       } else if (std::chrono::duration_cast<std::chrono::nanoseconds>(finish-lastPulse[i]).count() > 0.1*1000000000.0) { //acount for motors not spinning and holding last rpm value
              rpm_avg = 0.0;
              motors[i].setRpm(rpm_avg);
              rpm[i][2] = 0;
              rpm[i][1] = 0;
              rpm[i][0] = 0;
         }
	  } else if (rpm_time > 30) {
        rpm[i][2] = rpm[i][1];
        rpm[i][1] = rpm[i][0];
        rpm_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - deltaT).count();
       	rpm[i][0] = motors[i].getCount()/(rpm_time/1000 * scale)*60;

        rpm_avg = (rpm[i][0]+rpm[i][1]+rpm[i][2])/3;
        //float rpm_avg =  motors[i].getCount()/(rpm_time * scale)*60*1000;
        	motors[i].setRpm(rpm_avg);
        	motorPins[i].posCount = motorPins[i].posCount + motors[i].getCount();
        	motors[i].setCount(0);
	}
	if (rpm_time > 30) {
    myfile <<motors[i].getRpm()<<"   ";
    if (i == 3) {
      myfile <<std::endl;
    }
	float u = 0;
	bool pidFlag = true;
	//for good overall: kp = 1.7, ki = 1.2, kd = 0.02

  	error[i][1] = error[i][0];
  	error[i][0] = (motorPins[i].rpm - motors[i].getRpm());
  	integralError[i] += error[i][0]*rpm_time/1000;
  	derivativeError[i] = (error[i][0] - error[i][1]);


	if (motorPins[i].rpm < 15 && motorPins[i].rpm > -15) {
           kp = 2.75;
  	   ki = 0.02;
    	   kd = 0.02;
	   u = kp*error[i][0] + ki*integralError[i] + kd*derivativeError[i];
	   if (motors[i].getRpm() == 0 && motorPins[i].rpm != 0) {
		u = 15*sign(motorPins[i].rpm);
		pidFlag = false;
		//error[i][0] = 0;
		//error[i][1] = 0;
		integralError[i] = 0;
	   }
	} else {
	   kp = 1.1;
	   ki = 5.0;
	   kd = 0.02;
	}

		// if (motorPins[i].rpm != 0 && motors[i].getRpm() == 0.0) {
		// 	u = 15*sign(motorPins[i].rpm);
		// }else {
    //     	u = kp*error[i][0] + ki*integralError[i] + kd*derivativeError[i];
		// }
	if(pidFlag) {
   		u = kp*error[i][0] + ki*integralError[i] + kd*derivativeError[i];
        }
	if (u > 100)
        	u = 100;
        if (u < -100)
            u = -100;
	motorPins[i].strength = u;

  	if (motorPins[i].strength > 0 ) {
  		motors[i].forward(motorPins[i].strength);
  	} else if (motorPins[i].strength < 0) {
  		motors[i].backward((motorPins[i].strength*-1));
  	} else {
  		motors[i].stop();
    }
   }

  }
  if (rpm_time > 30) {
    deltaT = std::chrono::high_resolution_clock::now();
  }
  finish = std::chrono::high_resolution_clock::now();
}

  for (unsigned i = 0; i < 4; i++) {
    std::cout<<"Encoder: "<< i+1 <<std::endl;
    std::cout<<motors[i].getCount()<<std::endl;
    // std::cout<<encError[i]<<std::endl;
    printf("%f\n", motors[i].getRpm());
    printf("%d\n", motorPins[i].posCount);
  }

  myfile.close();

  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].stop();
  }
  delay(100);
}

int main() {
  float Vx = 0.0; //[mm/s]
  float Vy = 200.0;  //[mm/s] standard forward velocity for robot
  float Wz = 0.0;  //[rad/s]
  int CornerRad = 0;  //[mm]
  float delay_time = 0;

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
  while (1) {
    int trigger = digitalRead(30);
    if (trigger == 1)
      break;
  }

  //delay(1000);

   /*
   Vy = 200.0;
   Wz = 0.0;
   motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
   for (unsigned i = 0; i < motors.size(); i++) {
      motorPins[i].rpm =desired_rpm[i];
   }
   delay(2000);

   Vy = 100.0;
   Wz = 1.0;
   motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
   for (unsigned i = 0; i < motors.size(); i++) {
     motorPins[i].rpm = desired_rpm[i];
     motorPins[i].posCount = 0;
   }
   delay(1600);

   Vy = 200.0;
   Wz = 0;
   motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
   for (unsigned i = 0; i < motors.size(); i++) {
      motorPins[i].rpm = desired_rpm[i];
   }
   delay(1000);*/

   // delay(5000);
   // for (unsigned i = 0; i < 4; i++) {
   //     motorPins[i].posCount = 0;
   // }

  int trigger = 0;
  while (trigger < 3) {
     Vy = 200.0;
     Wz = 0.00;
     motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
     for (unsigned i = 0; i < motors.size(); i++) {
       motorPins[i].rpm = desired_rpm[i];
     }
     for (unsigned i = 0; i < 4; i++) {
       motorPins[i].posCount = 0;
     }
     int counter = 840*desired_rpm[0]/60*1.11;
     while (motorPins[0].posCount < counter) {
       delay(10);
     }
  
     //delay(1110+10);
  
     CornerRad = 100; //[mm]
     Vy = 100;
     trajectoryPlan(Wz, Vy, CornerRad, delay_time);
     motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
     for (unsigned i = 0; i < 4; i++) {
       motorPins[i].posCount = 0;
       motorPins[i].rpm = desired_rpm[i];
     }
     counter = 840*desired_rpm[0]/60*delay_time/1000;
     while (motorPins[0].posCount < counter) {
         delay(5);
     }
     trigger++;
   }
  end = true;
  delay(1000);

  t.detach();
  return 0;
}
