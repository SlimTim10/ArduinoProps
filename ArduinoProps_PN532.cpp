#include "ArduinoProps_PN532.h"

#include <maybe.h>

#include <PN532_SPI.h>
#include <PN532.h>
#include <SPI.h>

enum spi_settings {
	SPI_SPEED = 2000000,
	SPI_DATA_ORDER = LSBFIRST,
	SPI_DATA_MODE = SPI_MODE0,
};

static void configureSPI(void) {
	SPISettings settings = SPISettings(SPI_SPEED, SPI_DATA_ORDER, SPI_DATA_MODE);
	SPI.beginTransaction(settings);
}

enum rfid_errno initializeRFID(RFID *rfid, uint8_t attempts) {
	configureSPI();

	uint32_t versiondata = false;
	for (uint8_t i = 0; i < attempts && !versiondata; i++) {
		rfid->hardware->begin();
		versiondata = rfid->hardware->getFirmwareVersion();
	}
	if (!versiondata) {
		return RFID_ERRNO__INIT;
	}
	
	rfid->hardware->SAMConfig();
	return RFID_ERRNO__SUCCESS;
}

enum rfid_errno initializeRFIDs(RFID **rfids, uint8_t n, uint8_t initAttempts) {
	for (uint8_t i = 0; i < n; i++) {
		rfid_errno err = initializeRFID(rfids[i], initAttempts);
		if (err != RFID_ERRNO__SUCCESS) return err;
	}
}

/* (RFID, uint8_t) -> maybe uint8_t[] */
maybe readTag(RFID *rfid_, uint8_t timeout) {
	configureSPI();
	
	RFID *rfid = (RFID *) rfid_;
	
	uint8_t val[MIFAREULTRALIGHT_UID_LENGTH];
	uint8_t uidLength = MIFAREULTRALIGHT_UID_LENGTH;
	if (rfid->hardware->readPassiveTargetID(PN532_MIFARE_ISO14443A, val, &uidLength, timeout)) {
		if (rfid->hardware->mifareultralight_ReadPage(MIFAREULTRALIGHT_USER_PAGE1, rfid->tagData)) {
			return mreturn(rfid->tagData);
		}
	}
	return nothing();
}
