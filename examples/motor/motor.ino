#include "hal.h"
#include <RH_RF69.h>
#include <ArduinoProps.h>
#include <ArduinoProps_config.h>
#include <ArduinoProps_RF69.h>

#define DEBUG
#include <ArduinoProps_debug.h>

/* extern struct propInfo motorProp; */
struct propInfo motorProp = {
	.id = 0x01,
	.kind = 0x02,
};

static struct propInfo *prop = &motorProp;

extern uint8_t pingPayload[];
extern uint8_t pingPayloadLength;
static uint8_t motorPayload[] = { 0x05 };
static uint8_t motorPayloadLength = sizeof(motorPayload);

enum commands {
	CMD_NONE,
	CMD_PING,
	CMD_TOGGLE_MOTOR,
};

static RH_RF69 radio(RF_SS, RF_INT, RF_RESET);

void setup(void) {
	dbg_init();
	dbg_println("MOTOR PROP");

	initializeHAL();

	enum radio_errno err = initializeRadio(&radio);
	if (err == RADIO_ERRNO__INIT) {
		dbg_println("Radio initialization failed");
	}
	if (err == RADIO_ERRNO__SET_FREQ) {
		dbg_println("Radio setFrequency failed");
	}

	dbg_println("READY");
}

void loop(void) {
	if (radio.available()) {
		uint8_t packet[PACKET_MAX_LENGTH];
		uint8_t len = sizeof(packet);
		if (radio.recv(packet, &len)) {
			
			dbg_print("Received: ");
			/* Print as hex */
			for (uint8_t i = 0; i < len; i++) {
				dbg_printHex(packet[i]);
			}
			dbg_println();

			switch (parsePacket(packet)) {
			case CMD_PING:
				sendPing();
				break;
			case CMD_TOGGLE_MOTOR:
				toggleMotor();
				break;
			}
		}
	}
}

void toggleMotor(void) {
	dbg_println("Toggling motor");
	digitalWrite(MOTOR_RELAY, HIGH);
	delay(200);
	digitalWrite(MOTOR_RELAY, LOW);
}

enum commands parsePacket(uint8_t *packet) {
	if (matchPacket(prop, packet, pingPayload, pingPayloadLength)) {
		return CMD_PING;
	}
	if (matchPacket(prop, packet, motorPayload, motorPayloadLength)) {
		return CMD_TOGGLE_MOTOR;
	}
	return CMD_NONE;
}

void sendPing(void) {
	dbg_println("Sending ping");
	sendPayload(&radio, prop, pingPayload, pingPayloadLength);
}
