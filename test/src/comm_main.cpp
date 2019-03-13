#include <thread>
#include <fstream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "BLE.h"

void scanThread(BLE &ble) {
  ble.scan();
}

int main() {
  // unsigned long long sno = getSerialNumber();
  // system("sudo sh ../bash/ble_setup.sh");
  // system("sudo cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16- > serial_no.txt");
  FILE *pipe = popen("sudo cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16-", "r");
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
  std::cout << sno << std::endl;
  std::cout << usno << std::endl;
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, usno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  std::cout << ble.getUUID1() << std::endl;
  return 0;
  std::thread v;
  v = std::thread(scanThread, std::ref(ble));
  ble.send();
  auto start = std::chrono::high_resolution_clock::now();
  auto finish = start;
  int cnt = 1;
  while (1) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() > 3000) {
      ble.setUUID2((cnt*100/2));
      ble.setUUID3((cnt*100));
      ble.setUUID4(cnt);
      ble.send();
      cnt++;
      start = std::chrono::high_resolution_clock::now();
    }
    while (!ble.packets.empty()) {
      BLE t = ble.packets.front();
      ble.packets.pop();
      std::cout << "Packet received : UUID " << t.getUUID1() << ":" << t.getUUID2() << ":" << t.getUUID3() << ":" << t.getUUID4() \
      << " Major " << t.getMajor() << " Minor " << t.getMinor() << " Power " << t.getTxPower() << std::endl;
    }
    finish = std::chrono::high_resolution_clock::now();
  }
  return 0;
}
