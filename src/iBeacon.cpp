#include <cmath>
#include <sstream>

#include "iBeacon.h"

iBeacon::iBeacon () : ibeacon_1(0x0), ibeacon_2(0x0), uuid_1(0x0), uuid_2(0x0), \
                    uuid_3(0x0), uuid_4(0x0), maj(0x0), min(0x0), txPower(0x0) {}

iBeacon::iBeacon (unsigned long long ibeacon_1, unsigned long long ibeacon_2, \
        unsigned long long uuid_1, unsigned long long uuid_2, \
        unsigned long long uuid_3, unsigned long long uuid_4, \
        unsigned long maj, unsigned long min, unsigned long txPower) :
        ibeacon_1(ibeacon_1), ibeacon_2(ibeacon_2), uuid_1(uuid_1), \
        uuid_2(uuid_2), uuid_3(uuid_3), uuid_4(uuid_4), maj(maj), \
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
  if (packet.length() == 84) {
    // std::string ibeacon = packet.substr(0, 14);
    unsigned long long ibeacon_1 = hexToDec(packet.substr(0, 8));
    unsigned long long ibeacon_2 = hexToDec(packet.substr(8, 6));
    // std::string notsure = packet.substr(14, 26);
    unsigned long long ns_1 = hexToDec(packet.substr(14, 8));
    unsigned long long ns_2 = hexToDec(packet.substr(22, 8));
    unsigned long long ns_3 = hexToDec(packet.substr(30, 8));
    unsigned long ns_4 = hexToDec(packet.substr(38, 2));
    // std::string uuid = packet.substr(40, 32);
    // unsigned long long uuid = hexToDec(packet.substr(40, 32));
    unsigned long long uuid_1 = hexToDec(packet.substr(40, 8));
    unsigned long long uuid_2 = hexToDec(packet.substr(48, 8));
    unsigned long long uuid_3 = hexToDec(packet.substr(56, 8));
    unsigned long long uuid_4 = hexToDec(packet.substr(64, 8));
    // std::string major = packet.substr(72, 4);
    unsigned long major = hexToDec(packet.substr(72, 4));
    // std::string minor = packet.substr(76, 4);
    unsigned long minor = hexToDec(packet.substr(76, 4));
    // std::string power = packet.substr(80, 2);
    long txPower = hexToDec(packet.substr(80, 2));
    if ((txPower & 10000000) == 128) {
      txPower = twosComp(txPower);
    }
    // std::string notsure2 = packet.substr(82, 2);
    unsigned long ns = hexToDec(packet.substr(82, 2));

    iBeacon ibeacon (ibeacon_1, ibeacon_2, uuid_1, uuid_2, uuid_3, uuid_4, \
                    major, minor, txPower);
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
