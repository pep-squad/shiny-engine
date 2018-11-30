
#ifndef __BLE_H__
#define __BLE_H__

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <queue>

class BLE {
private:
  // parse the buffer to be in correct hexadecimal string
  void extractBuffer(char buffer[], std::queue<std::string> *packets); // extract the buffer into a vector of packets
protected:
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
  std::queue<std::string> packets;
  // Instantiate a new BLE packet
  BLE();
  BLE(unsigned long long packet);
  // get the values for all BLE packets
  void scan();
};

#endif
