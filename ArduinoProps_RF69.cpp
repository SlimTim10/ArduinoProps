#include "ArduinoProps.h"
#include "ArduinoProps_config.h"
#include "ArduinoProps_RF69.h"
#include "private.h"
#include <RH_RF69.h>

enum rf_settings {
	RF_POWER = 14,
	RF_PREAMBLE_LENGTH = 4,
	/* GFSK, Whitening, Rb = 2kbs,    Fd = 5kHz */
	/* RF_MODEM_CONFIG = RH_RF69::GFSK_Rb2Fd5, */
	/* GFSK, Whitening, Rb = 125kbs,  Fd = 125kHz */
	RF_MODEM_CONFIG = RH_RF69::GFSK_Rb125Fd125,
	/* GFSK, Whitening, Rb = 250kbs,  Fd = 250kHz */
	/* RF_MODEM_CONFIG = RH_RF69::GFSK_Rb250Fd250, */
};
const float RF_FREQ = 915.0;

enum radio_errno initializeRadio(RH_RF69 *radio) {
	if (!radio->init()) {
		return RADIO_ERRNO__INIT;
	}
	if (!radio->setFrequency(RF_FREQ)) {
		return RADIO_ERRNO__SET_FREQ;
	}
	
	radio->setTxPower(RF_POWER);

	extern uint8_t key[];
	radio->setEncryptionKey(key);

	radio->setModemConfig((RH_RF69::ModemConfigChoice) RF_MODEM_CONFIG);

	radio->setPreambleLength(RF_PREAMBLE_LENGTH);

	uint8_t syncwords[] = { 0x54, 0x4D };
	radio->setSyncWords(syncwords, sizeof(syncwords));

	return RADIO_ERRNO__SUCCESS;
}

bool matchPacket(struct propInfo *prop, uint8_t *recvPacket, uint8_t *payload, uint8_t payloadLength) {
	uint8_t header[] = { prop->id, prop->kind, payloadLength, HDR_RESERVED };
	uint8_t headerLength = sizeof(header);
	uint8_t packet[PACKET_MAX_LENGTH];
	uint8_t packetLength = headerLength + payloadLength;

	memcpy(packet, header, headerLength);
	memcpy(&packet[headerLength], payload, payloadLength);

	return (memcmp(recvPacket, packet, packetLength) == 0);
}

void sendPayload(RH_RF69 *radio, struct propInfo *prop, uint8_t *payload, uint8_t payloadLength) {
	uint8_t header[] = { prop->id, prop->kind, payloadLength, HDR_RESERVED };
	uint8_t headerLength = sizeof(header);
	uint8_t packet[PACKET_MAX_LENGTH];
	uint8_t packetLength = headerLength + payloadLength;

	memcpy(packet, header, headerLength);
	memcpy(&packet[headerLength], payload, payloadLength);
	
	radio->send(packet, packetLength);
	radio->waitPacketSent();
}

void sendPacket(RH_RF69 *radio, uint8_t *packet, uint8_t packetLength) {
	radio->send(packet, packetLength);
	radio->waitPacketSent();
}
