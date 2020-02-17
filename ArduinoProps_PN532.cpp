#include "ArduinoProps_PN532.h"

#include <maybe.h>

enum rfid_errno initializeRFID(PN532 *rfid, uint8_t attempts) {
	uint32_t versiondata = false;
	for (uint8_t i = 0; i < attempts && !versiondata; i++) {
		rfid->begin();
		versiondata = rfid->getFirmwareVersion();
	}
	if (!versiondata) {
		return RFID_ERRNO__INIT;
	}
	
	rfid->SAMConfig();

	return RFID_ERRNO__SUCCESS;
}

void initializeRFIDs(PN532 **rfids, uint8_t n, uint8_t initAttempts) {
	for (uint8_t i = 0; i < n; i++) {
		initializeRFID(rfids[i], initAttempts);
	}
}

/* (PN532, uint8_t) -> maybe uint8_t */
maybe readTag(PN532 *rfid_, uint8_t attempts) {
	PN532 *rfid = (PN532 *) rfid_;
	
	for (uint8_t i = 0; i < attempts; i++) {
		uint8_t val[MIFAREULTRALIGHT_UID_LENGTH];
		uint8_t uidLength = MIFAREULTRALIGHT_UID_LENGTH;
		if (rfid->readPassiveTargetID(PN532_MIFARE_ISO14443A, val, &uidLength)) {
			static uint8_t tagData[MIFAREULTRALIGHT_PAGE_SIZE];
			if (rfid->mifareultralight_ReadPage(MIFAREULTRALIGHT_USER_PAGE1, tagData)) {
				return mreturn(&tagData[0]);
			}
		}
	}
	return nothing();
}
