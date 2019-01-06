#include "Motor.h"

Motor::Motor(int pinEnable, int pinDirection1, int pinDirection2, pwm type) : \
	enableDisable(pinEnable), direction1(pinDirection1), direction2(pinDirection2), type(type) {
		if (type == HARDWARE) {
			setupHardPwm();
		} else if (type == SOFTWARE) {
			setupSoftPwm();
		}
}

void Motor::setupSoftPwm() {
	wiringPiSetup();

	pinMode(enableDisable, OUTPUT);
	pinMode(direction1, OUTPUT);
	pinMode(direction2, OUTPUT);

	digitalWrite(enableDisable, HIGH);
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
	digitalWrite(enableDisable, HIGH);
	softPwmWrite(direction1, strength);
	softPwmWrite(direction2, 0);
}

void Motor::softBackward(int strength) {
	digitalWrite(enableDisable, HIGH);
	softPwmWrite(direction1, 0);
	softPwmWrite(direction2, strength);
}

void Motor::stop() {
	digitalWrite(enableDisable, LOW);
	digitalWrite(direction1, LOW);
	digitalWrite(direction2, LOW);
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
