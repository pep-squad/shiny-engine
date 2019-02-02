#include "Motor.h"

Motor::Motor(int pinEnable, int pinDirection1, int pinDirection2, pwm type) : \
	enableDisable(pinEnable), direction1(pinDirection1), direction2(pinDirection2), type(type), pinNum(THREE), encType(OFF) {
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
 direction1(pinDirection1), direction2(pinDirection2), type(SOFTWARE), encoderA(encoderA), encoderB(encoderB), pinNum(TWO), encType(ON) {
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
	if (type == HARDWARE) {
		hardForward(strength);
	} else if (type == SOFTWARE) {
		softForward(strength);
	}
}

void Motor::backward(int strength) {
	if (type == HARDWARE) {
		hardBackward(strength);
	} else if (type == SOFTWARE) {
		softBackward(strength);
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
