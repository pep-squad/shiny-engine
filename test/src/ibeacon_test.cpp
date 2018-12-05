#include <cstdio>
#include <unistd.h>

#include "ibeacon_test.h"

TestPacket testpackets[BEACONS];
int sleepTime = 5000000;

void setTestPackets() {
  testpackets[0].maj = 0;
  testpackets[0].min = 0;
  testpackets[0].txPower = -128;

  testpackets[1].maj = 100;
  testpackets[1].min = 1;
  testpackets[1].txPower = 127;

  testpackets[2].maj = 12;
  testpackets[2].min = 12;
  testpackets[2].txPower = 0;

  testpackets[3].maj = 13;
  testpackets[3].min = 13;
  testpackets[3].txPower = 126;

  testpackets[4].maj = 3;
  testpackets[4].min = 2;
  testpackets[4].txPower = -127;

  testpackets[5].maj = 123;
  testpackets[5].min = 234;
  testpackets[5].txPower = -1;

  testpackets[6].maj = 555;
  testpackets[6].min = 444;
  testpackets[6].txPower = 1;

  testpackets[7].maj = 2323;
  testpackets[7].min = 3434;
  testpackets[7].txPower = 10;

  testpackets[8].maj = 0;
  testpackets[8].min = 111;
  testpackets[8].txPower = -100;

  testpackets[9].maj = 0;
  testpackets[9].min = 111;
  testpackets[9].txPower = 100;
}

void testIBeacon() {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  testIBeaconScan();
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  testIBeaconSend();
}

void testIBeaconSend() {
  // default iBeacon
  iBeacon def;
  def.send();
  usleep(sleepTime);
  // non-default iBeacons
  setTestPackets();
  for (size_t i = 0; i < BEACONS; i++) {
    std::cout << "Sending Packet " << i << " : major=" << testpackets[i].maj << \
    " : minor=" << testpackets[i].min << " : tx=" << testpackets[i].txPower << std::endl;
    iBeacon ndef (0x0, 0x0, 0x0, 0x0, 0x0, 0x0, testpackets[i].maj, testpackets[i].min, testpackets[i].txPower);
    ndef.send();
    usleep(sleepTime);
  }
}

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
