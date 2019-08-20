#include "ArduinoProps_PN532.h"

enum rfid_settings {
	RFID_INIT_ATTEMPTS = 3,
};

enum rfid_errno initializeRFID(PN532 *rfid) {
	uint32_t versiondata = false;
	for (uint8_t i = 0; i < RFID_INIT_ATTEMPTS && !versiondata; i++) {
		rfid->begin();
		versiondata = rfid->getFirmwareVersion();
	}
	if (!versiondata) {
		return RFID_ERRNO__INIT;
	}
	
	rfid->SAMConfig();

	return RFID_ERRNO__SUCCESS;
}
