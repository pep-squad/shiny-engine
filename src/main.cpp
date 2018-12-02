#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "iBeacon.h"

int main () {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  /*iBeacon ibeacon;
  ibeacon.scan();
  while (!ibeacon.packets.empty()) {
    iBeacon curr = ibeacon.packets.front();
    ibeacon.packets.pop();
    std::cout << curr.getMajor() << " : " << curr.getMinor() << " : " << curr.getTxPower() << std::endl;
  }*/

  iBeacon ibeacon (0,0,0,0,0,0,111,222,80);
  ibeacon.send();
  return 0;
}
