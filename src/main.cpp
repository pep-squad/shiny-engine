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
#include <map>
#include <list>
#include <csignal>

#include "BLE.h"
#include "Motor.h"
#include "TCS3200.h"
#include "HCSR04.h"

typedef struct DataPacket {
  unsigned long long eta;
  // unsigned long long position;
  unsigned long long serial;
  // unsigned long majorFlag;
} Packet;

void scanThread(BLE &ble) {
  ble.scan();
}

bool compareETA(std::pair<long unsigned int, DataPacket> one, std::pair<long unsigned int, DataPacket> two){
  if(one.second.eta == two.second.eta){
    if(one.second.serial < two.second.serial){
      return true;
    } else {
      return false;
    }
  } else if (one.second.eta < two.second.eta){
   return true;
  } else {
    return false;
  }
}

/*void calculatePosition(std::map<unsigned long, Packet>* botMap,BLE* ble){
  //list for ranking
  std::list<std::pair<unsigned long, Packet>> positionRankings;
  for(auto const& x: *botMap){
    positionRankings.push_back(x);
  }
  //add local to rankings list
  std::pair<unsigned long, Packet> local;
  local.first = ble->getUUID1();
  local.second.eta = ble->getUUID2();
  local.second.position = ble->getUUID3();
  local.second.serial = ble->getUUID1();
  positionRankings.push_back(local);
  //sort list
  positionRankings.sort(compareETA);
  //update positions and update map
  unsigned long long rank = 0;
  auto previous = positionRankings.front();
  for(auto & x: positionRankings){
    rank++;
    if (x.first != local.first) {
      x.second.position = rank;
      botMap->erase(x.first);
      botMap->insert(std::pair<unsigned long,Packet>(x.first, x.second));
    } else {
      //set threshold flag
      //if within 5 seconds of bot ahead of itself, slow down.
      if (local.second.position > 1) {
        //find previous list element
	float diff = local.second.position - previous.second.position;
        if (diff < 5 && diff > -5) {
          ble->setMajor(1);
        } else {
          ble->setMajor(0);
        }
      } else {
        ble->setMajor(0);
      }
      ble->setUUID3(rank);
    }
    previous = x;
  }
}*/

///////////////////MOTOR///////////////////////////

#define MINIMUM_DISTANCE  20

typedef enum TurningOptions {
  LEFT_TURN,
  RIGHT_TURN,
  STRAIGHT
} Turn;

typedef struct MotorPinLayout {
  int direction1, direction2, encoderA, encoderB;
  int strength, count, rpm, posCount;
} MotorPins;

double sign(double x) {
  if (x > 0.0) return 1.0;
  if (x < 0.0) return -1.0;
  return x;
}

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

void trajectoryPlan(float &Wz, float Vy, float rad, float &delay_time,float turn) {
    float pi = 3.14159;
    float alpha = rad*turn*pi/180;
    delay_time = alpha/Vy;
    //Wz = pi/(2*delay_time);
    Wz = sign(turn)*Vy/rad;
    delay_time = delay_time*1000;
    if (delay_time < 0) {
      delay_time *= -1;
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float etaVy(float Vy, std::pair<unsigned long, Packet> pckt, BLE ble) {
  float newVy;
  if (ble.getUUID1() < pckt.second.serial) {
    int diff = ble.getUUID2() - pckt.second.eta;
    if (diff <= -3 || diff >= 3) {
      newVy = Vy;
    } else {
      std::time_t epoch = std::time(nullptr);
      long int t = ble.getUUID2() - static_cast<long int>(epoch);
      float d = Vy * t;
      float tnew = static_cast<float>(t) + 3;
      newVy = d/tnew;
    }
  }
  return newVy;
}

float distanceVy(float Vy, float desired_Vy, float newDistance, float oldDistance) {
  /*if (newDistance < MINIMUM_DISTANCE) {
    if (newDistance > oldDistance) {
      // increase speed if getting further
      if (Vy > 0 && (Vy+1) <= desired_Vy) {
        Vy += 1;
      } else if (Vy < 0 && (Vy-1) >= desired_Vy) {
        Vy -= 1;
      }
    } else if (newDistance < oldDistance) {
      // decrease speed if getting closer
      if (Vy > 0) {
        if ((Vy-1) > 0) {
          Vy -= 1;
        } else {
          Vy = 0;
        }
      } else if (Vy < 0) {
        if ((Vy+1) > 0) {
          Vy += 1;
        } else {
          Vy = 0;
        }
      }
    }
  } else {
    Vy = desired_Vy;
  }*/
  if (newDistance < MINIMUM_DISTANCE) {
      // decrease the speed if getting closer
      if (desired_Vy > 0) {
        if ((Vy-5 > 0)) {
          Vy -= 5;
        } else {
          Vy = 0;
        }
      } else if (desired_Vy < 0) {
        if ((Vy+5) < 0) {
          Vy += 5;
        } else {
          Vy = 0;
        }
      }
  } else {
    Vy = desired_Vy;
  }
  return Vy;
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
  float rpm[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

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
        rpm[i][2] = rpm[i][1];
        rpm[i][1] = rpm[i][0];
        rpm[i][0] = motors[i].getCount()/(rpm_time/1000 * scale)*60;
        float rpm_avg = (rpm[i][0] + rpm[i][1])/2;
        motors[i].setRpm(rpm_avg);
        motorPins[i].posCount += motors[i].getCount();
        motors[i].setCount(0);
        if (rpm_avg > motorPins[i].rpm) {
          if ((rpm_avg-5) > motorPins[i].rpm) {
        motorPins[i].strength -= 2;
    } else {
              motorPins[i].strength--;
    }
        }
        else if(rpm_avg < motorPins[i].rpm) {
            if ((rpm_avg+5) < motorPins[i].rpm) {
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

  for (unsigned i = 0; i < motors.size(); i++) {
    motors[i].stop();
  }
  delay(100);
}

void testUltra(HCSR04 ultra) {
  // Testing Ultrasonic Sensor
  while(1) {
    float dist = ultra.distance();
    std::cout << dist << std::endl;
    usleep(25);
  }
}

void testColour (TCS3200 rgb) {
  // Testing the Colour Sensor
  int x = 0;
  while (1) {
    Colour col = rgb.scan();
    switch (col) {
      case RED:
        // std::cout << "RED " << x << std::endl;
        x++;
        break;
      case GREEN:
        x = 0;
        // std::cout << "GREEN\n";
        break;
      case WHITE:
        x = 0;
        // std::cout << "WHITE\n";
        break;
      case BLACK:
        x = 0;
        // std::cout << "BLACK\n";
      default:
        break;
    }
    usleep(25);
  }
}

long int timeToIntersection(int remTurns, float Vy, int currCount) {
  int straights = remTurns + 1;
  long int time;
  float remDistance = ((620.0 * (float)straights) - currCount) / 2.29;
  float time_f;
  time_f = (remDistance / Vy) + (remTurns * 1.47);
  if (time_f == 0.0) {
      time = 0;
  } else {
      std::time_t epoch = std::time(nullptr);
      long int epoch_int = static_cast<long int>(epoch);
      time = static_cast<long int>(time_f) + epoch_int;
  }
  return time;
}

int main(int argc, char const *argv[]) {
  long int time;
  /*MOTOR SETUP*/
  float Vx = 0.0; //[mm/s]
  float Vy = 0.0;  //[mm/s] standard forward velocity for robot
  float Wz = 0.0;  //[rad/s]
  HCSR04 ultra (9,7);
  TCS3200 rgb (12,13,14);
  std::thread t;
  std::vector<MotorPins> motorPins;
  std::vector<Motor> motors;
  Turn turns[4] = {LEFT_TURN,LEFT_TURN,LEFT_TURN,RIGHT_TURN};
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
  for (unsigned i = 0; i < motors.size(); i++) {
    motorPins[i].rpm = 0;
  }
  // testUltra(ultra);
  // testColour(rgb);
  float desired_Vy = 130.0;
  float max_Vy = desired_Vy;
  float distance = 0;
  int turnCount = 0;
  /*COMMS SETUP*/
  system("sh bash/ble_setup.sh");
  FILE *pipe = popen("cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16-", "r");
  std::map<unsigned long, Packet> m;
  std::array<char, 128> buffer;
  std::string sno;
  while (fgets(buffer.data(), 128, pipe) != NULL) {
      sno += buffer.data();
  }
  sno[sno.length()-1] = '\0';
  pclose(pipe);
  std::istringstream iss(sno);
  unsigned long usno;
  iss >> std::hex >> usno;
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, usno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  std::thread v;
  v = std::thread(scanThread, std::ref(ble));
  sleep(1);
  auto lastSend = std::chrono::high_resolution_clock::now();
  pid_t lastPid = -1;
  lastPid = ble.send();
  char ss;
  /*------CONTROL LOOP------*/
  do {
    for (unsigned i = 0; i < motors.size(); i++) {
      motorPins[i].rpm = 0;
      motors[i].stop();
      motorPins[i].posCount = 0;
    }
    ble.setUUID2(0xFFFFFF);
    ble.send();
    motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
    std::cout << "Continue(y/n)? ";
    std::cin >> ss;
    if (ss == 'y') {
      for (unsigned int cnt = 0; cnt < 9; cnt++) {
        /*Execute Motor control*/
        if ((turnCount%4) == 0) {
          Vy = desired_Vy; // reset the speed to desired speed after going through intersection
	  max_Vy = desired_Vy;
        }
        Vx = 0.0;
        Wz = 0.0;
        motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
        for (unsigned i = 0; i < motors.size(); i++) {
          motorPins[i].rpm = desired_rpm[i];
          motorPins[i].posCount = 0;
        }
        Colour currColour;
        bool white_flag = true;
        bool green_flag = true;
        bool red_flag = true;
        int corner = 0;
        int x = 0;
        float total = 0.0;
        while (red_flag) {
          total = 0.0;
          white_flag = true;
          green_flag = true;
	  if ((turnCount%4)!=3) {
              float dist = ultra.distance();
              Vy = distanceVy(Vy, max_Vy, dist, distance);
              distance = dist;
	  }
          currColour = rgb.scan();
          switch (currColour) {
            case RED:
            x++;
            if (corner > 10) {
              red_flag = false;
            }
            break;
            case GREEN:
            corner++;
            x = 0;
            green_flag = false;
            break;
            case WHITE:
            corner++;
            x = 0;
            white_flag = false;
            break;
            default:
            break;
          }
          if (!white_flag) {
            Vx = -15.0;
            Wz = 0.05;
          } else if (!green_flag) {
            Vx = 15.0;
            Wz = -0.05;
          } else {
            Vx = 0.0;
            Wz = 0.0;
          }
          int remTurns = 3-turnCount;
	  time = timeToIntersection(remTurns, Vy, total);
	  ble.setUUID2(int(time));
          /*Construct updated send message*/
          auto currentTime = std::chrono::high_resolution_clock::now();
          if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSend).count() > 500){
            if (lastPid > 0) {
              kill(lastPid, SIGKILL);
            }
            lastPid = ble.send();
            lastSend = std::chrono::high_resolution_clock::now();
          }
          /*COMMS routine for determining desired Vy*/
          while (!ble.packets.empty()) {
            BLE t = ble.packets.front();
            ble.packets.pop();
            unsigned long sno = t.getUUID1();
            Packet test = { t.getUUID2(), sno};
            m.erase(sno);
            m.insert(std::pair<unsigned long,Packet>(sno, test));
          }
	  if ((turnCount%4)!=3) {
            // calculatePosition(&m,&ble);
            for(auto const& pckt: m) {
              Vy = etaVy(Vy,pckt,ble);
	      max_Vy = Vy;
              std::cout << "         Serial: " << pckt.first << " ETA: " << pckt.second.eta << " new (Vy)   " << std::endl;
	      m.erase(pckt.first);
            }
            std::cout << "This Bot Serial: " << ble.getUUID1() << " ETA: " << ble.getUUID2() << " Speed (Vy) " << Vy << std::endl;
	  }
          // Update the vehicle speed based on the ultrasonic sensor, rgb sensor, and the intersection collision avoidance
          motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
	  total = 0.0;
          for (unsigned i = 0; i < 4; i++) {
            motorPins[i].rpm = desired_rpm[i];
            if (motorPins[i].posCount < 0) {
              total += (motorPins[i].posCount*-1);
            } else {
              total += motorPins[i].posCount;
            }
          }
          total /= 4;
          if (total >= 600.0) {
            red_flag = false;
          }
          usleep(10);
        }
        //////////// TURNING SECTION ////////////////
        Turn turn = turns[turnCount];
        if (turn == LEFT_TURN) {
          int CornerRad = 100;
          float delay_time = 0.0;
          float turn_Vy = 100.0;
          Vx = 0.0;
          trajectoryPlan(Wz, turn_Vy, CornerRad, delay_time,85.0);
          motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
          for (unsigned i = 0; i < 4; i++) {
            motorPins[i].rpm = desired_rpm[i];
            motorPins[i].posCount = 0;
          }
          int counter = 840*desired_rpm[0]/60*delay_time/1000;
          while(motorPins[0].posCount < counter) {
            delay(10);
          }
        } else if (turn == RIGHT_TURN) {
          int CornerRad = 100;
          float delay_time = 0.0;
          float turn_Vy = 100.0;
          Vx = 0.0;
          trajectoryPlan(Wz, turn_Vy, CornerRad, delay_time,-85.0);
          motorSpeed(Wz, Vx, Vy, std::ref(desired_rpm));
          for (unsigned i = 0; i < 4; i++) {
            motorPins[i].rpm = desired_rpm[i];
            motorPins[i].posCount = 0;
          }
          int counter = 840*desired_rpm[1]/60*delay_time/1000;
          while(motorPins[1].posCount > counter) {
            delay(10);
          }
        }
        turnCount = (turnCount+1)%4;
      }
      if (lastPid > 0) {
        kill(lastPid, SIGKILL);
      }
    }
  } while (ss == 'y');
  end = true;
  delay(1000);
  t.detach();
  v.detach();
  return 0;
}
