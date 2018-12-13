#include <stdio.h>

#include "hcsr04_test.h"

void testHCSR04() {
  HCSR04 d (4,5);
  while (1)
  {
      printf("Distance: %fcm\n", d.distance());
      delay(500);
  }
}
