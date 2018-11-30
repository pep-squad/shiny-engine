#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "iBeacon.h"

int main () {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  iBeacon ibeacon;
  ibeacon.scan();
  // BLE ble;
  // ble.scan();
  return 0;
}
