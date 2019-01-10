#include "hcsr04_test.h"

// std::vector<GPIO> pins;

void *hcsr04Scan(int threadid, std::vector<GPIO> pins, std::vector<std::vector<float>> &dist) {
  HCSR04 d (pins[(int)threadid].trig, pins[(int)threadid].echo);
  std::vector<float> distance;
  dist.push_back(distance);
  printf("%d %d-%d\n", (int)threadid, pins[(int)threadid].trig, pins[(int)threadid].echo);
  while (1) {
      printf("Distance: %fcm @ %d\n", d.distance(), (int)threadid);
      // dist[threadid].push_back(d.distance());
      // std::srand(time(0));
      // int del = (std::rand() % 100) + 50;
      delay(100);
  }
}

void testHCSR04() {
  std::vector<std::thread> v;
  std::vector<GPIO> pins;
  std::vector<std::vector<float>> d;
  GPIO tempPin;

  tempPin.trig = 24;
  tempPin.echo = 25;
  pins.push_back(tempPin);
  tempPin.trig = 22;
  tempPin.echo = 23;
  pins.push_back(tempPin);
  tempPin.trig = 29;
  tempPin.echo = 28;
  pins.push_back(tempPin);
  tempPin.trig = 0;
  tempPin.echo = 2;
  pins.push_back(tempPin);

  // for (size_t i = 0; i < pins.size(); i++) {
  for (size_t i = 2; i < 3; i++) {
    v.push_back(std::thread(hcsr04Scan, i, pins, std::ref(d)));
    delay(500);
  }

  while (1) {
    // for (unsigned i = 0; i < d.size(); i++) {
    //   unsigned size = d[i].size();
    //   for (unsigned j = 0; j < size; j++) {
    //     std::cout << "Thread " << i << " distance=" << d[i][j] << std::endl;
    //   }
    //   d[i].erase(d[i].begin(), d[i].begin() + size);
    // }
    // std::cout << std::endl << std::endl;
    // delay(1000);
  }
}
