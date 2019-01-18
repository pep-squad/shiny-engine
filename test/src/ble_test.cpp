#include <cstdio>

#include "ble_test.h"
#include "serial_number.h"

void *scanBLEPackets(BLE &ble) {
  while (1) {
    ble.scan();
    while (!ble.packets.empty()) {
      BLE s = ble.packets.front();
      ble.packets.pop();
      std::cout << "Packet received > " << s.getMajor() << " : " \
      << s.getMinor() << " : " << s.getTxPower() << std::endl;
    }
  }
}

void testBLEScan() {
  unsigned long long sno = getSerialNumber();
  // prefix1, prefix2, prefix3, uuid1, uuid2, uuid3, uuid4, major, minor, power
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, sno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  std::thread v;
  v = std::thread(scanBLEPackets, std::ref(ble));
  while(1);
}

void testBLESend() {
  // get the serial number
  unsigned long long sno = getSerialNumber();
  // prefix1, prefix2, prefix3, uuid1, uuid2, uuid3, uuid4, major, minor, power
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, sno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  ble.send();
}
