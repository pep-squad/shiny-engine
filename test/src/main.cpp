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
int EnableDisable_1=26;
int DirectionBit0_1=27;
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
	pwmWrite(EnableDisable_1,LOW);
	digitalWrite(DirectionBit0_1,LOW);
	digitalWrite(DirectionBit1_1,LOW);
	printf("exiting\n");
	exit(0);
}

// Function to trap "kill" command
void CatchKill(int c)
{
	pwmWrite(EnableDisable_2,LOW);
	digitalWrite(DirectionBit0_2,LOW);
	digitalWrite(DirectionBit1_2,LOW);
	pwmWrite(EnableDisable_1,LOW);
	digitalWrite(DirectionBit0_1,LOW);
	digitalWrite(DirectionBit1_1,LOW);
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
	pinMode(EnableDisable_1,PWM_OUTPUT);
	pinMode(DirectionBit0_1,OUTPUT);
	pinMode(DirectionBit1_1,OUTPUT);

	// Set up to catch <CTRL>C and "kill"
	signal(SIGINT,CatchCTRLC);
	signal(SIGTERM,CatchKill);

	while(1)
	{
// Forward
		pwmWrite(EnableDisable_1,LOW);
		pwmWrite(EnableDisable_2,LOW);
		delay(1000);
		printf("forward\n");
		digitalWrite(DirectionBit0_2,LOW);
		digitalWrite(DirectionBit1_2,HIGH);
		digitalWrite(DirectionBit0_1,LOW);
		digitalWrite(DirectionBit1_1,HIGH);
		pwmWrite(EnableDisable_2,512);
		pwmWrite(EnableDisable_1,2048);
		delay(2000);

// Reverse
		pwmWrite(EnableDisable_1,LOW);
		pwmWrite(EnableDisable_2,LOW);
		delay(1000);
		printf("reverse\n");
		digitalWrite(DirectionBit0_2,HIGH);
		digitalWrite(DirectionBit1_2,LOW);
		digitalWrite(DirectionBit0_1,HIGH);
		digitalWrite(DirectionBit1_1,LOW);
		pwmWrite(EnableDisable_2,1024);
		pwmWrite(EnableDisable_1,700);
		delay(2000);
	}

/*	// software PWM
	// Set up pins to all OUTPUT
		pinMode(EnableDisable,OUTPUT);
		pinMode(DirectionBit0,OUTPUT);
		pinMode(DirectionBit1,OUTPUT);
		digitalWrite(EnableDisable,LOW);
		int errPwm = softPwmCreate(EnableDisable, 0, 100);
		if (errPwm != 0) {
			fprintf(stdout, "ERROR\n");
			exit(0);
		}

		// Set up to catch <CTRL>C and "kill"
		signal(SIGINT,CatchCTRLC);
		signal(SIGTERM,CatchKill);
		while(1)
		{
	// Forward
			// digitalWrite(EnableDisable,LOW);
			softPwmWrite(EnableDisable, 100);
			delay(1000);
			printf("forward\n");
			digitalWrite(DirectionBit0,LOW);
			digitalWrite(DirectionBit1,HIGH);
			digitalWrite(EnableDisable,HIGH);
			delay(2000);

	// Reverse
			// digitalWrite(EnableDisable,LOW);
			softPwmWrite(EnableDisable, 25);
			delay(1000);
			printf("reverse\n");
			digitalWrite(DirectionBit0,HIGH);
			digitalWrite(DirectionBit1,LOW);
			digitalWrite(EnableDisable,HIGH);
			delay(2000);
		}*/

	return 0 ;
}
