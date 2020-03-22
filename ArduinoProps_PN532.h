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

typedef PN532_SPI RFIDHardwareSPI;
typedef PN532 RFIDHardware;

typedef struct {
	RFIDHardware *hardware;
	uint8_t tagData[MIFAREULTRALIGHT_PAGE_SIZE];
} RFID;

enum rfid_errno initializeRFID(RFID *, uint8_t);
enum rfid_errno initializeRFIDs(RFID **, uint8_t, uint8_t);
/* (PN532, uint8_t) -> maybe uint8_t[] */
maybe readTag(RFID *, uint8_t);

#endif // ARDUINOPROPS_PN532_H
