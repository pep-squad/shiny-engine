#include <thread>
#include <fstream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <list>

#include "BLE.h"

typedef struct DataPacket {
  unsigned long long eta;
  unsigned long long position;
  unsigned long majorFlag;

} Packet;

void scanThread(BLE &ble) {
  ble.scan();
}

bool compareETA(std::pair<long unsigned int, DataPacket> one, std::pair<long unsigned int, DataPacket> two){
  if(one.second.eta <= two.second.eta){
    return true;
  }
  else{
    return false;
  }
}

void calculatePosition(std::map<unsigned long, Packet>* botMap,BLE* ble){

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
  positionRankings.push_back(local);

  //sort list
  positionRankings.sort(compareETA);

  //update positions and update map
  unsigned long long rank = 0;
  auto previous = positionRankings.front();
  for(auto & x: positionRankings){
    rank++;

    if (x.first != local.first){
      x.second.position = rank;
      botMap->erase(x.first);
      botMap->insert(std::pair<unsigned long,Packet>(x.first, x.second));
    }
    else{
      //set threshold flag
      //if within 3 seconds of bot ahead of itself, slow down.
      if(local.second.position > 1){
        //find previous list element
        if((local.second.position - previous.second.position) <= 3000){
          ble->setMajor(1);
        }
        else{
          ble->setMajor(0);
        }
      }
      else{
        ble->setMajor(0);
      }

      ble->setUUID3(rank);
    }
    previous = x;
  }
}

int main() {

  system("sudo sh ../bash/ble_setup.sh");
  FILE *pipe = popen("sudo cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16-", "r");
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
  // unsigned long usno = std::stoull(sno, nullptr, 0);
  // unsigned long usno;
  // sscanf(sno.c_str(), "%lu", &usno);
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, usno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  std::thread v;
  v = std::thread(scanThread, std::ref(ble));
  ble.send();
  auto start = std::chrono::high_resolution_clock::now();
  auto finish = start;
  int cnt = 1;
  while (1) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() > 3000) {
      cnt++;
      ble.setUUID2((cnt*100/2));
      ble.setUUID3((cnt*100));
      ble.setUUID4(cnt);
      ble.send();
      start = std::chrono::high_resolution_clock::now();
    }
    bool empty = true;
    while (!ble.packets.empty()) {
      empty = false;
      BLE t = ble.packets.front();
      ble.packets.pop();
      unsigned long sno = t.getUUID1();
      // { eta, position }
      Packet test = { t.getUUID2(), t.getUUID3() };
      m.erase(sno);
      m.insert(std::pair<unsigned long,Packet>(sno, test));
      /*std::cout << "Packet received : UUID " << t.getUUID1() << ":" << t.getUUID2() << ":" << t.getUUID3() << ":" << t.getUUID4() \
      << " Major " << t.getMajor() << " Minor " << t.getMinor() << " Power " << t.getTxPower() << std::endl;*/
    }
    if (!empty) {
        for (auto itr = m.begin(); itr != m.end(); ++itr) {
            Packet temp = itr->second;
            std::cout << itr->first << " : " << temp.eta << ":" << temp.position << std::endl;
        }
    }
    finish = std::chrono::high_resolution_clock::now();
    calculatePosition(&m,&ble);
    for(auto const& entry: m){
      std::cout << "Serial: " << entry.first << " ETA: " << entry.second.eta << " Position: " << entry.second.position << " MajFlag: "<< entry.second.majorFlag << std::endl;
    }
    std::cout << "Serial: " << ble.getUUID1() << " ETA: " << ble.getUUID2() << " Position: " << ble.getUUID3() << " MajFlag: "<< ble.getMajor() << std::endl;
  }
  return 0;
}
