#include "ArduinoProps.h"
#include "ArduinoProps_RF69.h"
#include "ArduinoProps_RF69_private.h"
#include <RH_RF69.h>
#include <SPI.h>
#include <Arduino.h>
// #include <maybe.h>

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

static uint8_t *makePacket(Prop *prop, CommandID cmd, uint8_t *payload, uint8_t payloadLength) {
	Header header = { prop->address, cmd };
	size_t headerLength = sizeof(header);
	
	static uint8_t packet[PACKET_MAX_LENGTH];
	uint8_t packetLength = headerLength + payloadLength;

	size_t m = sizeof(header.address), n = sizeof(header.command);
	uint32_t address = __builtin_bswap32(header.address);
	memcpy(packet, &address, m);
	memcpy(&packet[m], &header.command, n);
	memcpy(&packet[headerLength], payload, payloadLength);

	return packet;
}

bool matchPayload(uint8_t *recvPacket, Prop *prop, CommandID cmd, uint8_t *payload, uint8_t payloadLength) {
	uint8_t *packet = makePacket(prop, cmd, payload, payloadLength);
	uint8_t packetLength = sizeof(Header) + payloadLength;

	return matchPacket(recvPacket, packet, packetLength);
}

bool matchPacket(uint8_t *recvPacket, uint8_t *packet, uint8_t packetLength) {
	return (memcmp(recvPacket, packet, packetLength) == 0);
}

void sendPayload(RH_RF69 *radio, Prop *prop, CommandID cmd, uint8_t *payload, uint8_t payloadLength) {
	uint8_t *packet = makePacket(prop, cmd, payload, payloadLength);
	uint8_t packetLength = sizeof(Header) + payloadLength;
	
	sendPacket(radio, packet, packetLength);
}

void sendPacket(RH_RF69 *radio, uint8_t *packet, uint8_t packetLength) {
	if (packetLength == 0) return;
	
	configureSPI();
	radio->send(packet, packetLength);
	radio->waitPacketSent();
}

/* RH_RF69 -> maybe PacketInfo */
maybe receivePacket(RH_RF69 *radio) {
	configureSPI();

	if (!radio->available()) return nothing();

	static uint8_t buf[PACKET_MAX_LENGTH];
	static PacketInfo packetInfo = {
		.value = buf,
	};
	// The length must be set to the maximum length we want to receive every time
	packetInfo.length = PACKET_MAX_LENGTH;
	
	return (radio->recv(packetInfo.value, &packetInfo.length))
		? mreturn(&packetInfo)
		: nothing();
}
