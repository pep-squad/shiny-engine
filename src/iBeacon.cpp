#include "iBeacon.h"

iBeacon::iBeacon () {
  advFlags = 0x0;
  advHeader = 0x0;
  companyID = 0x0;
  iBeaconType = 0x0;
  iBeaconLength = 0x0;
  uuid = 0x0;
  major = 0x0;
  minor = 0x0;
  txPower = 0x0;
}

iBeacon::iBeacon (unsigned long long packet) {}

void iBeacon::scan() {
  BLE ble;
  ble.scan();
  while (!ble.packets.empty()) {
    std::string s = ble.packets.front();
    ble.packets.pop();
    extractPacket(s);
  }
}

void iBeacon::extractPacket(std::string packet) {
  std::cout << packet << std::endl;
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
unsigned int iBeacon::getMajor() {
  return 0;
}
unsigned int iBeacon::getMinor() {
  return 0;
}
unsigned int iBeacon::getTxPower() {
  return 0;
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
