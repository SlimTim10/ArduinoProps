#include "hal.h"
#include <Arduino.h>

void initializeHAL(void) {
	pinMode(MOTOR_RELAY, OUTPUT);
	digitalWrite(MOTOR_RELAY, LOW);
}
