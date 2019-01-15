// #include "ble_test.h"
// #include "ibeacon_scan.h"
// #include "ibeacon_send.h"
// #include "hcsr04_test.h"
// #include "motor_test.h"
#include "serial_number.h"

int main(int argc, char const *argv[]) {
  // testBLE();
  // testIBeaconScan();
  // testHCSR04();
	// testMotors();

  unsigned long long serialno = getSerialNumber();
  std::cout << std::hex << serialno << std::endl;

  return 0;
}
