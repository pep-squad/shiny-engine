#include <thread>
#include <fstream>
#include <string>
#include <unistd.h>
#include <chrono>

#include "BLE.h"
#include "serial_number.h"

std::string header = "1e02011a1aff4c000215";

void sendPacket(BLE ble) {
    while(1) {
	ble.send();
	sleep(2);
    }
}

void extractBuffer(std::string buffer) {
    // packets->push(buffer);
    if (buffer.length() != 90) { return; }
    std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
    //std::string currHeader = buffer.substr(26, 20);
    // only add to iBeacons if the headers are the same
    //if (header.compare(currHeader) == 0)
    {
      // create and add iBeacon packet and add to packets list
      std::cout << buffer << "    :     " << buffer.length() << std::endl;
    }
}

void extractPacket(std::string packet) {
    if (packet.length() > 0) {
        std::string buffer = "";
	bool start = false;
	//std::cout << packet << " : @ " << time(0) << std::endl;
        for(unsigned i = 0; i < packet.length(); i++) {
	    if (!start) {
                if (packet[i] == '>') {
	            start = true;
		}
	    } else {
	        if (packet[i] == '>') {
		    extractBuffer(buffer);
		    //std::cout << buffer << std::endl;
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
  unsigned long cnt = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto end = start;  
  while(true) {
    std::ifstream ff("b.txt");
    //std::string out;
    char c;
    std::string str = "";
    unsigned long tCnt = 0;
    while(ff.get(c)) {
        if (c > 0 && tCnt >= cnt) {
            str += c;
	    tCnt++;
	}
        //std::cout << c;
    }
    cnt = tCnt;
    std::cout << cnt << std::endl;
    //std::cout << str << std::endl;
    extractPacket(str);
    /*if (ff.is_open()) {
      while (!ff.eof()) {
        ff >> out;
	if (out.length() > 0) {
            output.append(out);
	}
      }
      extractPacket(output);
    }*/
    ff.close();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() > 5000) {
	    std::cout << "CLEAR FILE\n";
      std::ofstream acf ("a.txt", std::ios_base::trunc);
      std::ofstream bcf ("b.txt", std::ios_base::trunc);
      acf.close();
      bcf.close();
      start = end;
      cnt = 0;
    }
    end = std::chrono::high_resolution_clock::now();
  }
  return 0;
}
