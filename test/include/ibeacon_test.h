#ifndef __IBEACON_TEST_H__
#define __IBEACON_TEST_H__

#include "iBeacon.h"

#define BEACONS 10

typedef struct testPacket {
  unsigned long long ibeacon_1, ibeacon_2;
  unsigned long long uuid_1, uuid_2, uuid_3, uuid_4;
  unsigned long maj;
  unsigned long min;
  long txPower;
} TestPacket;

extern TestPacket testpackets[BEACONS];
extern int sleepTime;

void setTestPackets();
void testIBeacon();
void testIBeaconSend();
void testIBeaconScan();

#endif
