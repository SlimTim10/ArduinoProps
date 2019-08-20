#ifndef ARDUINOPROPS_PN532_H
#define ARDUINOPROPS_PN532_H

#include <PN532_SPI.h>
#include <PN532.h>

enum rfid_errno {
	RFID_ERRNO__SUCCESS = 0,
	RFID_ERRNO__INIT,
};

enum rfid_errno initializeRadio(PN532 *);

#endif // ARDUINOPROPS_PN532_H
