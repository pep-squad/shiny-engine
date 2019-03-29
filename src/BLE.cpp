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
  std::istringstream iss(hex);
  unsigned long long usno;
  iss >> std::hex >> usno;
  return usno;
}

long twosComp(long tx) {
  for (int i = 0; i <= 7; i++) {
    tx ^= (1 << i);
  }
  tx += 1;
  tx *= -1;
  return tx;
}

pid_t BLE::send() {
  // 1E 02 01 1A 1A FF 4C 00 02 15 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 00 00 00 00 C8 00
  std::stringstream stringStream;
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
  // Send the packet
  stringStream << "hcitool -i hci0 cmd 0x08 0x0008 " << std::hex << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << p6 << " " << p7 << " " << p8 << " " << p9 << " " << p10 << " " << u1<< " " << u2 << " " << u3 << " " << u4 << " " << u5 << " " << u6 << " " << u7 << " " << u8 << " " << u9 << " " << u10 << " " << u11 << " " << u12 << " " << u13 << " " << u14 << " " << u15 << " " << u16<< " " << maj1 << " " << maj2 << " " << min1 << " " << min2 << " " << tx << " 00 > dump.txt &";
  std::string cmd = stringStream.str();
  // system(cmd.c_str());
  FILE *pipe = popen(cmd.c_str(), "r");
  pclose(pipe);
  return 0; 
}

void BLE::scan() {
  system("hcitool lescan --duplicates > /home/pi/shiny-engine/a.txt &");
  system("hcidump --raw > /home/pi/shiny-engine/b.txt &");
  unsigned long cnt = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto end = start;
  while (true) {
    std::ifstream ff("/home/pi/shiny-engine/b.txt");
    char c;
    std::string str = "";
    unsigned long tCnt = 0;
    while (ff.get(c)) {
      if (c > 0 && tCnt >= cnt) {
        str += c;
        tCnt++;
      }
    }
    cnt = tCnt;
    extractPacket(str);
    ff.close();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() > 10000) {
      std::ofstream acf ("/home/pi/shiny-engine/a.txt", std::ios_base::trunc);
      std::ofstream bcf ("/home/pi/shiny-engine/b.txt", std::ios_base::trunc);
      acf.close();
      bcf.close();
      start = end;
      cnt = 0;
    }
    usleep(1000000);
    end = std::chrono::high_resolution_clock::now();
  }
}

void BLE::extractPacket(std::string packet) {
  if (packet.length() > 0) {
    std::string buffer = "";
    bool start = false;
    for (unsigned i = 0; i < packet.length(); i++) {
      if (!start) {
        if (packet[i] == '>') {
          start = true;
        }
      } else {
        if (packet[i] == '>') {
          extractBuffer(buffer);
          buffer = "";
        } else {
          if ((packet[i] >= 48 && packet[i] <= 57) || (packet[i] >= 65 && packet[i] <= 70) || (packet[i] >= 97 && packet[i] <= 102)) {
            buffer += packet[i];
          }
        }
      }
    }
  }
}

void BLE::extractBuffer(std::string buffer) {
  if (buffer.length() != 90) { return; }
  std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
  std::string currHeader = buffer.substr(26, 20);
  // only add to iBeacons if the headers are the same
  if (header.compare(currHeader) == 0) {
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

void BLE::setMinor(unsigned long minor) {
   min = minor;
}
void BLE::setMajor(unsigned long major) {
   maj = major;
}

void BLE::setUUID1(unsigned long long uuid) {
  uuid1 = uuid;
}
void BLE::setUUID2(unsigned long long uuid) {
  uuid2 = uuid;
}
void BLE::setUUID3(unsigned long long uuid) {
  uuid3 = uuid;
}
void BLE::setUUID4(unsigned long long uuid) {
  uuid4 = uuid;
}
