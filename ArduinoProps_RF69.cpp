#include "ArduinoProps.h"
#include "ArduinoProps_config.h"
#include "ArduinoProps_RF69.h"
#include "ArduinoProps_RF69_private.h"
#include <RH_RF69.h>
#include <SPI.h>

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

enum spi_settings {
	SPI_SPEED = 1000000,
	SPI_DATA_ORDER = MSBFIRST,
	SPI_DATA_MODE = SPI_MODE0,
};

static void configureSPI(void) {
	SPISettings settings = SPISettings(SPI_SPEED, SPI_DATA_ORDER, SPI_DATA_MODE);
	SPI.beginTransaction(settings);
}

enum radio_errno initializeRadio(RH_RF69 *radio) {
	configureSPI();
	
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

static uint8_t *makePacket(Prop *prop, uint8_t *payload, uint8_t payloadLength) {
	Header header = { prop->address, payloadLength };
	size_t headerLength = sizeof(header);
	
	uint8_t packet[PACKET_MAX_LENGTH];
	uint8_t packetLength = headerLength + payloadLength;

	size_t m = sizeof(header.address), n = sizeof(header.payloadLength);
	memcpy(packet, &header.address, m);
	memcpy(&packet[m], &header.payloadLength, n);
	memcpy(&packet[headerLength], payload, payloadLength);

	return packet;
}

bool matchPayload(Prop *prop, uint8_t *recvPacket, uint8_t *payload, uint8_t payloadLength) {
	uint8_t *packet = makePacket(prop, payload, payloadLength);
	uint8_t packetLength = sizeof(Header) + payloadLength;

	return matchPacket(recvPacket, packet, packetLength);
}

bool matchPacket(uint8_t *recvPacket, uint8_t *packet, uint8_t packetLength) {
	return (memcmp(recvPacket, packet, packetLength) == 0);
}

void sendPayload(RH_RF69 *radio, Prop *prop, uint8_t *payload, uint8_t payloadLength) {
	uint8_t *packet = makePacket(prop, payload, payloadLength);
	uint8_t packetLength = sizeof(Header) + payloadLength;

	sendPacket(radio, packet, packetLength);
}

void sendPacket(RH_RF69 *radio, uint8_t *packet, uint8_t packetLength) {
	configureSPI();
	radio->send(packet, packetLength);
	radio->waitPacketSent();
}
