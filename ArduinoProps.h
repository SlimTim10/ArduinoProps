#ifndef ARDUINOPROPS_H
#define ARDUINOPROPS_H

#include <stdint.h>

#define die()	while (1)

struct propInfo {
	uint8_t id;
	uint8_t kind;
};

enum extra {
	HDR_RESERVED = 0x00,
};

#endif // ARDUINOPROPS_H
