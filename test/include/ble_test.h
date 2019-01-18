#ifndef __BLE_TEST_H__
#define __BLE_TEST_H__

#include <thread>

#include "BLE.h"

void *scanBLEPackets(BLE &ble);
void testBLEScan();
void testBLESend();

#endif
