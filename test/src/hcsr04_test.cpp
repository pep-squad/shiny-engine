#include <cstdio>
#include <iostream>
#include <pthread.h>
#include "hcsr04_test.h"

typedef struct HCSR04GPIOPin {
  int trig;
  int echo;
} GPIO;

GPIO pins[2];

void *hcsr04Scan(void *threadid) {
  HCSR04 d (pins[(int)threadid].trig, pins[(int)threadid].echo);
  printf("%d %d-%d\n", (int)threadid, pins[(int)threadid].trig, pins[(int)threadid].echo);
  while (1)
  {
      printf("Distance: %fcm @ %d\n", d.distance(), (int)threadid);
      delay(500);
  }
}

void testHCSR04() {
  pthread_t threads[2];
  int rc;

  // pins[0].trig = 1;
  // pins[0].echo = 4;
  // pins[1].trig = 7;
  // pins[1].echo = 0;
  pins[0].trig = 1;
  pins[0].echo = 4;
  pins[1].trig = 7;
  pins[1].echo = 0;

  for (size_t i = 0; i < 2; i++) {
    rc = pthread_create(&threads[i], NULL, hcsr04Scan, (void*)i);

    if (rc) {
      std::cout << "FAILED " << i << std::endl;
    }
  }
  pthread_exit(NULL);
}
