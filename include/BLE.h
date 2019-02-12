#ifndef __BLE_H__
#define __BLE_H__

#include <cstdio>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cmath>

class BLE {
private:
  unsigned long long prefix1, prefix2;
  unsigned int prefix3;
  unsigned long long uuid1, uuid2, uuid3, uuid4;
  unsigned long maj, min;
  long txPower;
  std::string header;
  void extractPacket(std::string buffer);
  void extractBuffer(std::string buffer);
protected:
  // Set the packet structure
  /*void setAdvFlags(unsigned int advFlags);
  void setAdvHeader(unsigned int advHeader);
  void setCompanyID(unsigned int companyID);
  void setiBeaconType(unsigned int type);
  void setiBeaconLength(unsigned int length);
  void setUUID(unsigned int uuid);
  void setMajor(unsigned int maj);
  void setMinor(unsigned int minor);
  void setTxPower(unsigned int txPower);*/
public:
  #define NUM_PIPES          2

  #define PARENT_WRITE_PIPE  0
  #define PARENT_READ_PIPE   1

  int pipes[NUM_PIPES][2];

  #define READ_FD  0
  #define WRITE_FD 1

  #define PARENT_READ_FD  ( pipes[PARENT_READ_PIPE][READ_FD]   )
  #define PARENT_WRITE_FD ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )

  #define CHILD_READ_FD   ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
  #define CHILD_WRITE_FD  ( pipes[PARENT_READ_PIPE][WRITE_FD]  )
  // queue to hold all of the BLE packets
  std::queue<BLE> packets;
  // Instantiate a new BLE packet
  BLE();
  BLE (unsigned long long prefix1, unsigned long long prefix2, unsigned int prefix3, \
          unsigned long long uuid1, unsigned long long uuid2, \
          unsigned long long uuid3, unsigned long long uuid4, \
          unsigned long maj, unsigned long minor, unsigned long txPower);

  void scan();
  void send();
  unsigned long long getPrefix1();
  unsigned long long getPrefix2();
  unsigned long long getUUID1();
  unsigned long long getUUID2();
  unsigned long long getUUID3();
  unsigned long long getUUID4();
  unsigned long getMajor();
  unsigned long getMinor();
  long getTxPower();
  void setMinor(unsigned long major);
  void setMajor(unsigned long minor);
};

#endif
