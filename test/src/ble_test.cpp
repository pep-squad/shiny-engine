#include <cstdio>

#include "ble_test.h"

void testBLE() {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  testBLEScan();
}

void testBLEScan() {
  BLE ble;
  ble.scan();
  while (!ble.packets.empty()) {
    std::string s = ble.packets.front();
    ble.packets.pop();
    std::cout << s << std::endl;
  }
}
