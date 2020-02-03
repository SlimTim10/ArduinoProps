#include "hal.h"
#include <RH_RF69.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <ArduinoProps.h>
#include <ArduinoProps_config.h>
#include <ArduinoProps_RF69.h>
#include <ArduinoProps_PN532.h>

#define DEBUG
#include <ArduinoProps_debug.h>

/* extern struct propInfo rfidProp; */
struct propInfo rfidProp = {
	.id = 0x01,
	.kind = 0x01,
};

static struct propInfo *prop = &rfidProp;

extern uint8_t pingPayload[];
extern uint8_t pingPayloadLength;

enum commands {
	CMD_NONE,
	CMD_PING,
};

static RH_RF69 radio(RF_SS, RF_INT, RF_RESET);
static PN532_SPI pn532spi(SPI, RFID_SS);
static PN532 rfid(pn532spi);

void setup(void) {
	dbg_init();
	dbg_println("RFID PROP");

	initializeRadio(&radio);
	initializeRFID(&rfid);

	dbg_println("READY");
}

void loop(void) {
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
	uint8_t uidLength;
	uint8_t rfidRead = rfid.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

	if (rfidRead) {
		dbg_println("RFID tag found");
		dbg_print("UID length: "); dbg_println(uidLength);
		dbg_print("UID: ");
		rfid.PrintHex(uid, uidLength); dbg_println();
		uint8_t data[4];
		rfidRead = rfid.mifareultralight_ReadPage(4, data);
		if (rfidRead) {
			dbg_print("Page 4: ");
			rfid.PrintHex(data, 4); dbg_println();
			sendPayload(&radio, prop, data, 1);

			/* Workaround? */
			delay(500);
			initializeRadio(&radio);
			initializeRFID(&rfid);
		}
	}
}
