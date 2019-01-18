#include "BLE.h"
// sudo apt-get install libboost-all-dev
BLE::BLE () : \
        prefix1(0x0), prefix2(0x0), prefix3(0x0), uuid1(0x0), uuid2(0x0), \
        uuid3(0x0), uuid4(0x0), maj(0x0), min(0x0), txPower(0x0) {}

BLE::BLE (unsigned long long prefix1, unsigned long long prefix2, unsigned int prefix3, \
        unsigned long long uuid1, unsigned long long uuid2, \
        unsigned long long uuid3, unsigned long long uuid4, \
        unsigned long maj, unsigned long min, unsigned long txPower) :
        prefix1(prefix1), prefix2(prefix2), prefix3(prefix3), uuid1(uuid1), \
        uuid2(uuid2), uuid3(uuid3), uuid4(uuid4), maj(maj), \
        min(min), txPower(txPower) {
          std::stringstream hex1, hex2, hex3;
          int size;
          std::string p1, p2, p3;

          hex1 << std::hex << prefix1;
          p1 = hex1.str();
          size = p1.length();
          if (size != 8) {
            for (int i = 0; i < (8-size); i++) {
              p1.insert(0, "0");
            }
          }

          hex2 << std::hex << prefix2;
          p2 = hex2.str();
          size = p2.length();
          if (size != 8) {
            for (int i = 0; i < (8-size); i++) {
              p2.insert(0, "0");
            }
          }

          hex3 << std::hex << prefix3;
          p3 = hex3.str();
          size = p3.length();
          if (size != 4) {
            for (int i = 0; i < (4-size); i++) {
              p3.insert(0, "0");
            }
          }

          header = p1;
          header.append(p2);
          header.append(p3);
        }

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

void BLE::send() {
  // 1E 02 01 1A 1A FF 4C 00 02 15 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 00 00 00 00 C8 00
  std::stringstream stringStream;
  std::stringstream test;
  std::string cmd;
  long tx = txPower;
  if (tx < 0) {
    tx = twosComp(tx);
  }
  // prefix1
  unsigned int p1 = (prefix1 & 0xFF000000) >> 24U;
  unsigned int p2 = (prefix1 & 0x00FF0000) >> 16U;
  unsigned int p3 = (prefix1 & 0x0000FF00) >> 8U;
  unsigned int p4 = (prefix1 & 0x000000FF);
  // prefix2
  unsigned int p5 = (prefix2 & 0xFF000000) >> 24U;
  unsigned int p6 = (prefix2 & 0x00FF0000) >> 16U;
  unsigned int p7 = (prefix2 & 0x0000FF00) >> 8U;
  unsigned int p8 = (prefix2 & 0x000000FF);
  // prefix3
  unsigned int  p9 = (prefix3 & 0xFF00) >> 8U;
  unsigned int  p10 = (prefix3 & 0x00FF);
  // uuid1
  unsigned int u1 = (uuid1 & 0xFF000000) >> 24U;
  unsigned int u2 = (uuid1 & 0x00FF0000) >> 16U;
  unsigned int u3 = (uuid1 & 0x0000FF00) >> 8U;
  unsigned int u4 = (uuid1 & 0x000000FF);
  // uuid2
  unsigned int u5 = (uuid2 & 0xFF000000) >> 24U;
  unsigned int u6 = (uuid2 & 0x00FF0000) >> 16U;
  unsigned int u7 = (uuid2 & 0x0000FF00) >> 8U;
  unsigned int u8 = (uuid2 & 0x000000FF);
  // uuid3
  unsigned int u9 = (uuid3 & 0xFF000000) >> 24U;
  unsigned int u10 = (uuid3 & 0x00FF0000) >> 16U;
  unsigned int u11 = (uuid3 & 0x0000FF00) >> 8U;
  unsigned int u12 = (uuid3 & 0x000000FF);
  //uuid4
  unsigned int u13 = (uuid4 & 0xFF000000) >> 24U;
  unsigned int u14 = (uuid4 & 0x00FF0000) >> 16U;
  unsigned int u15 = (uuid4 & 0x0000FF00) >> 8U;
  unsigned int u16 = (uuid4 & 0x000000FF);
  // major
  unsigned int maj1 = (maj & 0xFF00) >> 8U;
  unsigned int maj2 = (maj & 0x00FF);
  // minor
  unsigned int min1 = (min & 0xFF00) >> 8U;
  unsigned int min2 = (min & 0x00FF);
  // 1e 02 01 1a 1a ff FF FF 02 15
  stringStream << "sudo /home/pi/shiny-engine/bash/ble_send.sh ' " \
  << std::hex << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " \
  << p6 << " " << p7 << " " << p8 << " " << p9 << " " << p10 << " " \
  << u1 << " " << u2 << " " << u3 << " " << u4 << " " \
  << u5 << " " << u6 << " " << u7 << " " << u8 << " " \
  << u9 << " " << u10 << " " << u11 << " " << u12 << " " \
  << u13 << " " << u14 << " " << u15 << " " << u16 << " " \
  << maj1 << " " << maj2 << " " << min1 << " " << min2 << " " << tx << " 00'";
  cmd = stringStream.str();
  system(cmd.c_str());
}

void BLE::scan() {
  // pipes for parent to write and read
  pipe(pipes[PARENT_READ_PIPE]);
  pipe(pipes[PARENT_WRITE_PIPE]);

  pid_t pid  = fork();
  if(pid > 0) {
    // child process
    dup2(CHILD_READ_FD, STDIN_FILENO);
    dup2(CHILD_WRITE_FD, STDOUT_FILENO);

    // close fds not required by child
    close(CHILD_READ_FD);
    close(CHILD_WRITE_FD);
    close(PARENT_READ_FD);
    close(PARENT_WRITE_FD);

    // execute the bash script to read ble packets
    execl("/bin/bash","bash","/home/pi/shiny-engine/bash/ble_scan.sh", NULL); // change to some common place later (/usr/bin)
  } else if (pid == 0) {
    // parent process
    char buffer[200];
    int count;

    // close fds not required by parent
    close(CHILD_READ_FD);
    close(CHILD_WRITE_FD);

    // Write to child’s stdin
    write(PARENT_WRITE_FD, "2^32\n", 5);

    // Read from child’s stdout
    while((count = read(PARENT_READ_FD, buffer, sizeof(buffer)-1)) > 0) {
      buffer[count] = 0; // get rid of bad data at end
      // buffer[count-1] = '\0'; // get rid of newline
      extractBuffer(buffer);
    }
  }
}

void BLE::extractBuffer(std::string buffer) {
  buffer.erase(remove_if(buffer.begin(), buffer.end(), isspace), buffer.end());
  if (buffer[0] == '>') {
    buffer = buffer.substr(1, buffer.length());
    // packets->push(buffer);
    if (buffer.length() != 90) { return; }
    std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
    std::string currHeader = buffer.substr(26, 20);
    // only add to iBeacons if the headers are the same
    if (header.compare(currHeader)) { return; }
    {
      std::string data = buffer.substr(46, buffer.length());
      // prefix
      unsigned int prefix1 = hexToDec(header.substr(0, 8));
      unsigned int prefix2 = hexToDec(header.substr(8, 8));
      unsigned int prefix3 = hexToDec(header.substr(16, 2));
      // uuid
      unsigned long long uuid1 = hexToDec(data.substr(0, 8));
      unsigned long long uuid2 = hexToDec(data.substr(8, 8));
      unsigned long long uuid3 = hexToDec(data.substr(16, 8));
      unsigned long long uuid4 = hexToDec(data.substr(24, 8));

      unsigned long maj = hexToDec(data.substr(32, 4)); // major
      unsigned long min = hexToDec(data.substr(36, 4)); // minor
      int txPower = hexToDec(data.substr(40, 2)); // power
      if ((txPower & 10000000) == 128) { // convert to negative if necessary
        txPower = twosComp(txPower);
      }
      // create and add iBeacon packet and add to packets list
      BLE ble (prefix1, prefix2, prefix3, uuid1, uuid2, uuid3, uuid4, maj, min, txPower);
      packets.push(ble);
    }
  }
}

unsigned long long BLE::getPrefix1() {
  return prefix1;
}
unsigned long long BLE::getPrefix2() {
  return prefix2;
}
unsigned long long BLE::getUUID1() {
  return uuid1;
}
unsigned long long BLE::getUUID2() {
  return uuid2;
}
unsigned long long BLE::getUUID3() {
  return uuid3;
}
unsigned long long BLE::getUUID4() {
  return uuid4;
}
unsigned long BLE::getMajor() {
  return maj;
}
unsigned long BLE::getMinor() {
  return min;
}
long BLE::getTxPower() {
  return txPower;
}
