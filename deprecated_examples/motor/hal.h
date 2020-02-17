#ifndef _HAL_H
#define _HAL_H

enum pins {
	RF_SS = 9,
	RF_INT = 2,
	RF_RESET = 3,
	MOTOR_RELAY = 7,
};

void initializeHAL(void);

#endif
