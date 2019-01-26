#include "TCS3200.h"

int main(int argc, char const *argv[]) {
  TCS3200 rgb(4,5,6);

  while (1) {
    RGB a = rgb.scan();
    std::cout << a.red << ":" << a.green << ":" << a.blue << std::endl;
  }
}
