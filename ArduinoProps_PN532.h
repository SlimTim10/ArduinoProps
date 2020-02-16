#ifndef ARDUINOPROPS_PN532_H
#define ARDUINOPROPS_PN532_H

#include <PN532_SPI.h>
#include <PN532.h>

#include <maybe.h>

enum rfid_errno {
	RFID_ERRNO__SUCCESS = 0,
	RFID_ERRNO__INIT,
};

enum {
	MIFAREULTRALIGHT_UID_LENGTH = 7,
	MIFAREULTRALIGHT_PAGE_SIZE = 4,
	MIFAREULTRALIGHT_USER_PAGE1 = 4,
};

enum rfid_errno initializeRFID(PN532 *);
void initializeRFIDs(PN532 **, uint8_t, uint8_t);
/* (PN532, uint8_t) -> maybe uint8_t[MIFAREULTRALIGHT_PAGE_SIZE] */
maybe readTag(PN532 *, uint8_t);
/* uint8_t[] -> maybe void */
maybe printData(void *);

#endif // ARDUINOPROPS_PN532_H
