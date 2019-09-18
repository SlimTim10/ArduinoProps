#ifndef ARDUINOPROPS_H
#define ARDUINOPROPS_H

#include <stdint.h>

struct propInfo {
	uint8_t id;
	uint8_t kind;
};

enum extra {
	HDR_RESERVED = 0x00,
};

#endif // ARDUINOPROPS_H
