#include <thread>
#include <fstream>
#include <string>

#include "BLE.h"
#include "serial_number.h"

std::string header = "1e02011a1aff4c000215";

void extractBuffer(std::string buffer) {
    // packets->push(buffer);
    if (buffer.length() <= 90) { return; }
    std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
    std::string currHeader = buffer.substr(26, 20);
    // only add to iBeacons if the headers are the same
    if (header.compare(currHeader))
    {
      // create and add iBeacon packet and add to packets list
      std::cout << buffer << "    :     " << buffer.length() << std::endl;
    }
}

void extractPacket(std::string packet) {
    if (packet.length() > 0) {
        std::string buffer = "";
	bool start = false;
        for(unsigned i = 0; i < packet.length(); i++) {
	    if (!start && packet[i] == '>') {
	        start = true;
	    } else if (start && packet[i] == '>') {
		extractBuffer(buffer);
	        buffer = "";
	    } else if (start) {
	        buffer += packet[i];
	    }
	}
    }
}


int main(int argc, char const *argv[]) {
//  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
/*
  unsigned long long sno = getSerialNumber();
  BLE ble (0x1e02011a, 0x1aff4c00, 0x0215, sno, 0x0, 0x0, 0x0, 0xde, 0x6f, 0x78);
  system("sudo hcitool lescan --duplicates > /home/pi/a.txt &");
  system("sudo hcidump --raw > /home/pi/b.txt &");
  while (1) {
      ble.scan();
  }
  */
  
  system("sudo hcitool lescan --duplicates > a.txt &");
  system("sudo hcidump --raw > b.txt &");
  while(true) {
    std::ifstream ff;
    ff.open("b.txt");
    std::string out;
    std::string output = "";
    if (ff.is_open()) {
      while (!ff.eof()) {
        ff >> out;
	output.append(out);
      }
      extractPacket(output);
    }
    ff.close();
    std::ofstream acf ("a.txt", std::ios_base::trunc);
    std::ofstream bcf ("b.txt", std::ios_base::trunc);
    acf.close();
    bcf.close();
  }
  return 0;
}
