#include "Motor.h"

Motor::Motor(int pinEnable, int pinDirection1, int pinDirection2, pwm type) : \
	enableDisable(pinEnable), direction1(pinDirection1), direction2(pinDirection2), type(type), pinNum(THREE) {
		if (type == HARDWARE) {
			setupHardPwm();
		} else if (type == SOFTWARE) {
			setupSoftPwm();
		}
}

Motor::Motor(int pinDirection1, int pinDirection2) : \
 direction1(pinDirection1), direction2(pinDirection2), type(SOFTWARE), pinNum(TWO), encType(OFF) {
	setupSoftPwm();
}

Motor::Motor(int pinDirection1, int pinDirection2, int encoderA, int encoderB) : \
 direction1(pinDirection1), direction2(pinDirection2), encoderA(encoderA), encoderB(encoderB), type(SOFTWARE), pinNum(TWO), encType(ON), \
 newA(0), newB(0), oldA(0), oldB(0), count(0), rpm(0) {
	setupSoftPwmEncoder();
}

void Motor::setupSoftPwm() {
	wiringPiSetup();

	if (pinNum == THREE) {
		pinMode(enableDisable, OUTPUT);
		digitalWrite(enableDisable, HIGH);
	}

	pinMode(direction1, OUTPUT);
	pinMode(direction2, OUTPUT);
	int one = softPwmCreate(direction1, 0, 100);
	int two = softPwmCreate(direction2, 0, 100);

	if (one != 0 || two != 0) {
		printf("ERROR\n");
		// return an error code
	}
}

void Motor::setupSoftPwmEncoder() {
	wiringPiSetup();

	if (pinNum == THREE) {
		pinMode(enableDisable, OUTPUT);
		digitalWrite(enableDisable, HIGH);
	}

	pinMode(direction1, OUTPUT);
	pinMode(direction2, OUTPUT);
	pinMode(encoderA, INPUT);
	pinMode(encoderB, INPUT);

	int one = softPwmCreate(direction1, 0, 100);
	int two = softPwmCreate(direction2, 0, 100);

	if (one != 0 || two != 0) {
		printf("ERROR\n");
		// return an error code
	}
}

void Motor::setupHardPwm() {
	wiringPiSetup();

	pinMode(enableDisable, PWM_OUTPUT);
	pinMode(direction1, OUTPUT);
	pinMode(direction2, OUTPUT);
}

void Motor::hardForward(int strength) {
	pwmWrite(enableDisable, LOW);
	delay(1000);
	digitalWrite(direction1, LOW);
	digitalWrite(direction2, HIGH);
	pwmWrite(enableDisable, strength);
}

void Motor::hardBackward(int strength) {
	pwmWrite(enableDisable, LOW);
	delay(10);
	digitalWrite(direction1, HIGH);
	digitalWrite(direction2, LOW);
	pwmWrite(enableDisable, strength);
}

void Motor::softForward(int strength) {
	if (pinNum == THREE) {
		digitalWrite(enableDisable, HIGH);
	}
	softPwmWrite(direction1, strength);
	softPwmWrite(direction2, 0);
}

void Motor::softBackward(int strength) {
	if (pinNum == THREE) {
		digitalWrite(enableDisable, HIGH);
	}
	softPwmWrite(direction1, 0);
	softPwmWrite(direction2, strength);
}

void Motor::softStop() {
	if (pinNum == THREE) {
		digitalWrite(enableDisable, LOW);
	}
	softPwmWrite(direction1, 0);
	softPwmWrite(direction2, 0);
}

void Motor::hardStop() {
	digitalWrite(enableDisable, LOW);
	digitalWrite(direction1, LOW);
	digitalWrite(direction2, LOW);
}

void Motor::stop() {
	if (type == HARDWARE) {
		hardStop();
	} else if (type == SOFTWARE) {
		softStop();
	}
}

void Motor::forward(int strength) {
	if (strength > 100) {
		strength = 100;
	} else if (strength < 0) {
		strength = 0;
	}
	if (type == HARDWARE) {
		hardForward(strength);
	} else if (type == SOFTWARE) {
		softForward(strength);
	}
}

void Motor::backward(int strength) {
	if (strength > 100) {
		strength = 100;
	} else if (strength < 0) {
		strength = 0;
	}
	if (type == HARDWARE) {
		hardBackward(strength);
	} else if (type == SOFTWARE) {
		softBackward(strength);
	}
}

void Motor::encoderStage(int encVal) {
	switch (encVal) {
    case 1:
    case 7:
    case 8:
    case 14:
    //forward direciton
			count++;
      break;
    case 2:
    case 4:
    case 11:
    case 13:
    //reverse direction
			count--;
      break;
	}
}

int Motor::getEncoderA() {
	if (encType == ON) {
		return digitalRead(encoderA);
	}
	return -1;
}

int Motor::getEncoderB() {
	if (encType == ON) {
		return digitalRead(encoderB);
	}
	return -1;
}

void Motor::setOldA(int val) {
	oldA = val;
}
void Motor::setOldB(int val) {
	oldB = val;
}
void Motor::setNewA(int val) {
	newA = val;
}
void Motor::setNewB(int val) {
	newB = val;
}
void Motor::setCount(int val) {
	count = val;
}
void Motor::setRpm(float val) {
	rpm = val;
}

void Motor::setDirection1(int pin) {
	direction1 = pin;
}

void Motor::setDirection2(int pin) {
	direction2 = pin;
}

void Motor::setEncoderA(int pin) {
	encoderA = pin;
}

void Motor::setEncoderB(int pin) {
	encoderB = pin;
}

int Motor::getOldA() {
	return oldA;
}
int Motor::getOldB() {
	return oldB;
}
int Motor::getNewA() {
	return newA;
}
int Motor::getNewB() {
	return newB;
}
int Motor::getCount() {
	return count;
}
float Motor::getRpm() {
	return rpm;
}
