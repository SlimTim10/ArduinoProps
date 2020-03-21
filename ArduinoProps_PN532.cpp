#include "ArduinoProps_PN532.h"

#include <maybe.h>

enum rfid_errno initializeRFID(RFID *rfid, uint8_t attempts) {
	uint32_t versiondata = false;
	for (uint8_t i = 0; i < attempts && !versiondata; i++) {
		rfid->pn532->begin();
		versiondata = rfid->pn532->getFirmwareVersion();
	}
	if (!versiondata) {
		return RFID_ERRNO__INIT;
	}
	
	rfid->pn532->SAMConfig();
	return RFID_ERRNO__SUCCESS;
}

enum rfid_errno initializeRFIDs(RFID **rfids, uint8_t n, uint8_t initAttempts) {
	for (uint8_t i = 0; i < n; i++) {
		rfid_errno err = initializeRFID(rfids[i], initAttempts);
		if (err != RFID_ERRNO__SUCCESS) return err;
	}
}

/* (RFID, uint8_t) -> maybe uint8_t[] */
maybe readTag(RFID *rfid_, uint8_t attempts) {
	RFID *rfid = (RFID *) rfid_;
	
	for (uint8_t i = 0; i < attempts; i++) {
		uint8_t val[MIFAREULTRALIGHT_UID_LENGTH];
		uint8_t uidLength = MIFAREULTRALIGHT_UID_LENGTH;
		if (rfid->pn532->readPassiveTargetID(PN532_MIFARE_ISO14443A, val, &uidLength)) {
			if (rfid->pn532->mifareultralight_ReadPage(MIFAREULTRALIGHT_USER_PAGE1, rfid->tagData)) {
				return mreturn(rfid->tagData);
			}
		}
	}
	return nothing();
}
