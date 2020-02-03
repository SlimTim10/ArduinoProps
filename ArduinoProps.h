#ifndef ARDUINOPROPS_H
#define ARDUINOPROPS_H

#include <stdint.h>

typedef struct {
	uint32_t id;
} Prop;

typedef struct {
	uint32_t id;
	uint8_t payloadLength;
} Header;

#endif // ARDUINOPROPS_H
