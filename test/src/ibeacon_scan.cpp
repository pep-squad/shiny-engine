#include <cstdio>
#include <unistd.h>

#include "ibeacon_scan.h"

int sleepTime = 5000000;

void testIBeaconScan() {
  iBeacon ibeacon;
  ibeacon.scan();
  for (size_t i = 0; !ibeacon.packets.empty(); i++) {
    iBeacon curr = ibeacon.packets.front();
    ibeacon.packets.pop();
    std::cout << "Packet " << i << " received > " << curr.getMajor() << " : " \
     << curr.getMinor() << " : " << curr.getTxPower() << std::endl;
  }
}
