#ifndef ARDUINOPROPS_H
#define ARDUINOPROPS_H

#include <stdint.h>

typedef enum {
	payloadInt = 0x01,
	payloadIntList = 0x02,
	payloadString = 0x03,
	pingCmd = 0x80,
} CommandID;

typedef struct {
	uint32_t address;
} Prop;

typedef struct {
	uint32_t address;
	CommandID command;
} Header;

#endif // ARDUINOPROPS_H
