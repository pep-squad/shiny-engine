#include "ble_test.h"

int main(int argc, char const *argv[]) {
  system("sudo /home/pi/shiny-engine/bash/ble_setup.sh");
  // testBLEScan();
  testBLESend();
  return 0;
}
