#ifndef __IBEACON_H__
#define __IBEACON_H__

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <queue>

#include "BLE.h"

/************************************************
iBeacon Packet Structure
__________________________________________________
| iBeacon | UUID     | Major   | Minor   | TX     |
| Prefix  |          | Number  | Number  | Power  |
| 9 Bytes | 16 Bytes | 2 Bytes | 2 Bytes | 1 Byte |
__________________________________________________

iBeacon Prefix Structure
_______________________________________________
| Adv   | Adv    | Company | iBeacon | iBeacon |
| Flags | Header | ID      | Type    | Length  |
| 3 B   | 2 B    | 2 B     | 1 B     | 1 B     |
_______________________________________________


- Adv Flags
    -- 0x020116 > BLE General Discoverable and BR/EDR high-speed incompatibility

- UUID, Major, and Minor can all be used for user data

- For more information, visit:
    -- https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
************************************************/

class iBeacon : protected BLE {
private:
  // unsigned int advFlags;
  // unsigned int advHeader;
  // unsigned int companyID;
  // unsigned int iBeaconType;
  // unsigned int iBeaconLength;
  unsigned long long ibeacon_1, ibeacon_2;
  unsigned long long uuid_1, uuid_2, uuid_3, uuid_4;
  unsigned long maj;
  unsigned long min;
  long txPower;
  void extractPacket(std::string packet, std::queue<iBeacon> *packets);
protected:
  // Set the packet structure
  void setAdvFlags(unsigned int advFlags);
  void setAdvHeader(unsigned int advHeader);
  void setCompanyID(unsigned int companyID);
  void setiBeaconType(unsigned int type);
  void setiBeaconLength(unsigned int length);
  void setUUID(unsigned int uuid);
  void setMajor(unsigned int maj);
  void setMinor(unsigned int minor);
  void setTxPower(unsigned int txPower);
public:
  iBeacon ();
  iBeacon (unsigned long long ibeacon_1, unsigned long long ibeacon_2, \
          unsigned long long uuid_1, unsigned long long uuid_2, \
          unsigned long long uuid_3, unsigned long long uuid_4, \
          unsigned long maj, unsigned long minor, unsigned long txPower);
  std::queue<iBeacon> packets;
  void scan();
  void send();
  unsigned int getAdvFlags();
  unsigned int getAdvHeader();
  unsigned int getCompanyID();
  unsigned int getiBeaconType();
  unsigned int getiBeaconLength();
  unsigned int getUUID();
  unsigned long getMajor();
  unsigned long getMinor();
  long getTxPower();
};
#endif
