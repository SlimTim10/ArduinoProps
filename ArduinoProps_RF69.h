#ifndef ARDUINOPROPS_RF69_H
#define ARDUINOPROPS_RF69_H

#include "ArduinoProps.h"
#include <RH_RF69.h>

enum radio_errno {
	RADIO_ERRNO__SUCCESS = 0,
	RADIO_ERRNO__INIT,
	RADIO_ERRNO__SET_FREQ,
};

typedef RH_RF69 Radio;

enum radio_errno initializeRadio(RH_RF69 *);
bool matchPayload(uint8_t *, Prop *, CommandID, uint8_t *, uint8_t);
bool matchPacket(uint8_t *, uint8_t *, uint8_t);
void sendPayload(RH_RF69 *, Prop *, CommandID, uint8_t *, uint8_t);
void sendPacket(RH_RF69 *, uint8_t *, uint8_t);

#endif // ARDUINOPROPS_RF69_H
