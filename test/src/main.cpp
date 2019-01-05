// #include "ble_test.h"
// #include "ibeacon_test.h"
// #include "hcsr04_test.h"

/*int main(int argc, char const *argv[]) {
  // testBLE();
  // testIBeacon();
  testHCSR04();
  return 0;
}*/




// http://www.noveldevices.co.uk/rp-project-motor-control

/****************************************************************************
* rp-motor-control.c                                                        *
*                                                                           *
* Uses a Texas Instruments L293D IC to drive a motor in forward and reverse *
*                                                                           *
* Source -> http://www.noveldevices.co.uk/rpdl/rp-motor-control.c           *
*                                                                           *
* Written by JST Lawrence, Novel Devices.                                   *
*                                                                           *
* Notes:                                                                    *
* Uses the L293D Enable/Disable feature to avoid the risk of shorting the   *
* supply. Traps CTRL-C so that outputs are off before the program ends.     *
*                                                                           *
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

// Three GPIOs are used
// 1st - Enable/Disable signal
// 2nd - bit 0 of 2-bit signal to control direction
// 3rd - bit 1 of 2-bit signal to control direction

//	Using Pi B+ or B2
int EnableDisable_1=1;
int DirectionBit0_1=29;
int DirectionBit1_1=28;

int EnableDisable_2=23;
int DirectionBit0_2=24;
int DirectionBit1_2=25;

// Function to trap <CTRL>C
void CatchCTRLC(int c)
{
	pwmWrite(EnableDisable_2,LOW);
	digitalWrite(DirectionBit0_2,LOW);
	digitalWrite(DirectionBit1_2,LOW);
	printf("exiting\n");
	exit(0);
}

// Function to trap "kill" command
void CatchKill(int c)
{
	pwmWrite(EnableDisable_2,LOW);
	digitalWrite(DirectionBit0_2,LOW);
	digitalWrite(DirectionBit1_2,LOW);
	printf("exiting because a kill command was issued\n");
	exit(0);
}

int main (int argc, char *argv[])
{
	wiringPiSetup();

// Set up pins to all OUTPUT
	pinMode(EnableDisable_2,PWM_OUTPUT);
	pinMode(DirectionBit0_2,OUTPUT);
	pinMode(DirectionBit1_2,OUTPUT);

	// Set up to catch <CTRL>C and "kill"
	signal(SIGINT,CatchCTRLC);
	signal(SIGTERM,CatchKill);

	while(1)
	{
// Forward
		pwmWrite(EnableDisable_2,LOW);
		delay(1000);
		printf("forward\n");
		digitalWrite(DirectionBit0_2,LOW);
		digitalWrite(DirectionBit1_2,HIGH);
		pwmWrite(EnableDisable_2,512);
		delay(2000);

// Reverse
		pwmWrite(EnableDisable_2,LOW);
		delay(1000);
		printf("reverse\n");
		digitalWrite(DirectionBit0_2,HIGH);
		digitalWrite(DirectionBit1_2,LOW);
		pwmWrite(EnableDisable_2,1024);
		delay(2000);
	}

	return 0 ;
}
