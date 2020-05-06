#ifndef ARDUINOPROPS_H
#define ARDUINOPROPS_H

#include <stdint.h>

#define die() while (1)

enum prop_settings {
	PACKET_MAX_LENGTH = 252,
};

typedef enum {
	CMD_PAYLOAD_INT = 0x01,
	CMD_PAYLOAD_INT_LIST = 0x02,
	CMD_PAYLOAD_STRING = 0x03,
	CMD_PING = 0x80,
} CommandID;

typedef struct {
	uint32_t address;
} Prop;

typedef struct {
	uint32_t address;
	/* CommandID as uint8_t */
	uint8_t command;
} Header;

#endif // ARDUINOPROPS_H
