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
// MOTOR 1
int EnableDisable=26; // WiringPi 26, physical 35
int DirectionBit0=27; // WiringPi 27, physical 36
int DirectionBit1=28; // WiringPi 28, physical 37
// MOTOR 2
/*int EnableDisable=23; // WiringPi 23, physical 32
int DirectionBit0=24; // WiringPi 24, physical 36
int DirectionBit1=25; // WiringPi 25, physical 38*/

// Function to trap <CTRL>C
void CatchCTRLC()
{
	digitalWrite(EnableDisable,LOW);
	digitalWrite(DirectionBit0,LOW);
	digitalWrite(DirectionBit1,LOW);
	printf("exiting\n");
	exit(0);
}

// Function to trap "kill" command
void CatchKill()
{
	digitalWrite(EnableDisable,LOW);
	digitalWrite(DirectionBit0,LOW);
	digitalWrite(DirectionBit1,LOW);
	printf("exiting because a kill command was issued\n");
	exit(0);
}

int main (int argc, char *argv[])
{
	wiringPiSetup();

// Set up pins to all OUTPUT
	pinMode(EnableDisable,OUTPUT);
	pinMode(DirectionBit0,OUTPUT);
	pinMode(DirectionBit1,OUTPUT);

	// Set up to catch <CTRL>C and "kill"
	signal(SIGINT,CatchCTRLC);
	signal(SIGTERM,CatchKill);

	while(1)
	{
// Forward
		digitalWrite(EnableDisable,LOW);
		delay(1000);
		printf("forward\n");
		digitalWrite(DirectionBit0,LOW);
		digitalWrite(DirectionBit1,HIGH);
		digitalWrite(EnableDisable,HIGH);
		delay(2000);

// Reverse
		digitalWrite(EnableDisable,LOW);
		delay(1000);
		printf("reverse\n");
		digitalWrite(DirectionBit0,HIGH);
		digitalWrite(DirectionBit1,LOW);
		digitalWrite(EnableDisable,HIGH);
		delay(2000);
	}

	return 0 ;
}
