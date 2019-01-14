#include <cmath>
#include <sstream>

#include "iBeacon.h"

iBeacon::iBeacon () : \
        prefix1(0x0), prefix2(0x0), prefix3(0x0), uuid1(0x0), uuid2(0x0), \
        uuid3(0x0), uuid4(0x0), maj(0x0), min(0x0), txPower(0x0) {}

iBeacon::iBeacon (unsigned long long prefix1, unsigned long long prefix2, unsigned int prefix3, \
        unsigned long long uuid1, unsigned long long uuid2, \
        unsigned long long uuid3, unsigned long long uuid4, \
        unsigned long maj, unsigned long min, unsigned long txPower) :
        prefix1(prefix1), prefix2(prefix2), prefix3(prefix3), uuid1(uuid1), \
        uuid2(uuid2), uuid3(uuid3), uuid4(uuid4), maj(maj), \
        min(min), txPower(txPower) {}

unsigned long long hexToDec(std::string hex) {
  unsigned long long dec;
  dec = 0;
  int size = hex.length();
  for (int i = 1; i <= size; i++) {
    int j = size - i;
    if (hex[j] >= 48 && hex[j] <= 57) {
      dec += (hex[j] - 48) * pow(16, (i-1));
    } else if (hex[j] >= 65 && hex[j] <= 70) {
      dec += (hex[j] - 55) * pow(16, (i-1));
    }
  }
  return dec;
}

long twosComp(long tx) {
  for (int i = 0; i <= 7; i++) {
    tx ^= (1 << i);
  }
  tx += 1;
  tx *= -1;
  return tx;
}

void iBeacon::scan() {
  BLE ble;
  ble.scan();
  while (!ble.packets.empty()) {
    std::string s = ble.packets.front();
    ble.packets.pop();
    extractPacket(s, &packets);
  }
}

void iBeacon::send() {
  // 1E 02 01 1A 1A FF 4C 00 02 15 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 00 00 00 00 C8 00
  std::stringstream stringStream;
  std::string cmd;
  long tx = txPower;
  if (tx < 0) {
    tx = twosComp(tx);
  }
  stringStream << "sudo /home/pi/shiny-engine/bash/ble_send.sh '1e 02 01 1a 1a ff 4c 00 02 15 " << \
  std::hex << "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" << std::hex << \
  " 00 " << maj << " 00 " << std::hex << min << " " << std::hex << tx << " 00'";
  // std::cout << stringStream.str() << std::endl;
  cmd = stringStream.str();
  system(cmd.c_str());
}

void iBeacon::extractPacket(std::string packet, std::queue<iBeacon> *packets) {
  // temp packet to look for
  // 043E2A02010300D184BBEB27B8 1E 02011A1AFF4C000215636F3F8F64914BEE95F7D8CC64A863B500000000C8 D0
  if (packet.length() == 90) {
    std::string header = packet.substr(0, 28);
    std::string data = packet.substr(28, packet.length());

    // prefix
    unsigned int prefix1 = hexToDec(data.substr(0, 8));
    unsigned int prefix2 = hexToDec(data.substr(8, 8));
    unsigned int prefix3 = hexToDec(data.substr(16, 2));
    // uuid
    unsigned long long uuid1 = hexToDec(data.substr(18, 8));
    unsigned long long uuid2 = hexToDec(data.substr(26, 8));
    unsigned long long uuid3 = hexToDec(data.substr(34, 8));
    unsigned long long uuid4 = hexToDec(data.substr(42, 8));

    unsigned long maj = hexToDec(data.substr(50, 4)); // major
    unsigned long min = hexToDec(data.substr(54, 4)); // minor
    int txPower = hexToDec(data.substr(58, 2)); // power
    if ((txPower & 10000000) == 128) { // convert to negative if necessary
      txPower = twosComp(txPower);
    }
    // create and add iBeacon packet to vector
    iBeacon ibeacon (prefix1, prefix2, prefix3, uuid1, uuid2, uuid3, uuid4, maj, min, txPower);
    packets->push(ibeacon);
  }
}

unsigned int iBeacon::getAdvFlags() {
  return 0;
}
unsigned int iBeacon::getAdvHeader() {
  return 0;
}
unsigned int iBeacon::getCompanyID() {
  return 0;
}
unsigned int iBeacon::getiBeaconType() {
  return 0;
}
unsigned int iBeacon::getiBeaconLength() {
  return 0;
}
unsigned int iBeacon::getUUID() {
  return 0;
}
unsigned long iBeacon::getMajor() {
  return maj;
}
unsigned long iBeacon::getMinor() {
  return min;
}
long iBeacon::getTxPower() {
  return txPower;
}

void iBeacon::setAdvFlags(unsigned int advFlags) {
}
void iBeacon::setAdvHeader(unsigned int advHeader) {
}
void iBeacon::setCompanyID(unsigned int companyID) {
}
void iBeacon::setiBeaconType(unsigned int type) {
}
void iBeacon::setiBeaconLength(unsigned int length) {
}
void iBeacon::setUUID(unsigned int uuid) {
}
void iBeacon::setMajor(unsigned int major) {
}
void iBeacon::setMinor(unsigned int minor) {
}
void iBeacon::setTxPower(unsigned int txPower) {
}
