#include <thread>
#include <fstream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <string>

#include "BLE.h"

void scanThread(BLE &ble) {
  ble.scan();
}

int main() {
  // unsigned long long sno = getSerialNumber();
  system("sudo cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16- > serial_no.txt");
  return 0;
  std::ifstream fsno ("serial_no.txt");
  std::string sno;
  std::getline(fsno, sno);
  unsigned long usno = std::stoul(sno, nullptr, 0);
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, usno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  std::thread v;
  v = std::thread(scanThread, std::ref(ble));
  while (1) {
    while (!ble.packets.empty()) {
      BLE t = ble.packets.front();
      ble.packets.pop();
      std::cout << "Packet received : " << t.getUUID1() << t.getUUID2() << t.getUUID3() << t.getUUID4() \
      << " : " << t.getMajor() << " : " << t.getMinor() << " : " << t.getTxPower() << std::endl;
    }
  }
  return 0;
}
