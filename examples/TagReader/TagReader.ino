#include "address.h"
#ifndef PROP_ADDRESS
  #define PROP_ADDRESS 0x00000000
#endif

#include "hal.h"

#include <ArduinoProps.h>
#include <ArduinoProps_RF69.h>
#include <ArduinoProps_PN532.h>

#define DEBUG
#include <ArduinoProps_debug.h>

#include <Arduino.h>

#include <maybe.h>
#include <functional.h>

static Prop prop = {
	.address = PROP_ADDRESS,
};

extern uint8_t pingPayload[];
extern uint8_t pingPayloadLength;

/* All timings are in milliseconds unless otherwise specified */
enum settings {
	RFID_INIT_ATTEMPTS = 3,
	TAG_FIND_TIMEOUT = 100,
	READ_INTERVAL = 500,
};

enum typical_payload {
	PAYLOAD_LENGTH = 1,
};

static Radio radio(RF_CS, RF_G0, RF_RST);

static RFIDHardwareSPI rfidHardwareSPI(RFID_SS);
static RFIDHardware rfidHardware(rfidHardwareSPI);
static RFID rfid;

void setup(void) {
	dbg_init();
	dbg_println("Initializing prop: TagReader");

	enum radio_errno radio_err = initializeRadio(&radio);
	if (radio_err != RADIO_ERRNO__SUCCESS) {
		dbg_print("Radio initialization failed. Error code: "); dbg_println(radio_err);
		die();
	}

	rfid.hardware = &rfidHardware;
	
	enum rfid_errno rfid_err = initializeRFID(&rfid, RFID_INIT_ATTEMPTS);
	if (rfid_err != RFID_ERRNO__SUCCESS) {
		dbg_print("RFID initialization failed. Error code: "); dbg_println(rfid_err);
		die();
	}

	dbg_println("READY");
}

void loop(void) {
	CommandID cmd = CMD_PAYLOAD_INT;
	
	maybe tag = readTag(&rfid, TAG_FIND_TIMEOUT);
	uint8_t payload[PAYLOAD_LENGTH];
	payload[0] = encode(tag);

	dbg_print("Sending: ");
	forEach(payload, PAYLOAD_LENGTH, dbg_print); dbg_println();
	sendPayload(&radio, &prop, cmd, payload, PAYLOAD_LENGTH);

	delay(READ_INTERVAL);
}

uint8_t encode(maybe tag) {
	if (tag.just) {
		return *(uint8_t *) tag.just;
	} else {
		return 0x00;
	}
}
