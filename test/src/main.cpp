#include "ble_test.h"
// #include "hcsr04_test.h"
// #include "motor_test.h"
// #include "serial_number.h"

int main(int argc, char const *argv[]) {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  // testBLEScan();
  testBLESend();
  // testHCSR04();
	// testMotors();

  // unsigned long long serialno = getSerialNumber();
  // std::cout << std::hex << serialno << std::endl;

  return 0;
}
