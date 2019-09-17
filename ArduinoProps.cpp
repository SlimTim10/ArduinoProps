#include "ArduinoProps.h"

struct propInfo hallwayDoorTouchPanel = {
	.id = 0x01,
	.kind = 0x01,
};

struct propInfo hallwayDoor = {
	.id = 0x01,
	.kind = 0x02,
};

// Common command payloads
uint8_t pingPayload[] = { 0xFF };
uint8_t pingPayloadLength = sizeof(pingPayload);
