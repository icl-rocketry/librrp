#include "lora_sx1280.h"
#include <libriccore/riccorelogging.h>

volatile bool LoRaSX1280::receivedFlag = false;

LoRaSX1280::LoRaSX1280(int cs, int irq, int rst, int gpio, SPIClass& spi):
	module(cs, irq, rst, gpio, spi),
	sx1280(&module)
{}

bool LoRaSX1280::setup()
{
    delay(1500);

	if (sx1280.begin() != RADIOLIB_ERR_NONE)
	{
		RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: driver setup failed!");
		return (false);
	}
	sx1280.setPacketReceivedAction(setFlag);
	m_config = m_defaultConfig;

    if (sx1280.setFrequency(m_config.frequency) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected frequency is invalid for this module!");
		return (false);
    }

    if (sx1280.setBandwidth(m_config.bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected bandwidth is invalid for this module!");
        return (false);
    }

    if (sx1280.setSpreadingFactor(m_config.spreadingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected spreading factor is invalid for this module!");
        return (false);
    }

    if (sx1280.setCodingRate(m_config.codingRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected coding rate is invalid for this module!");
        return (false);
    }

    if (sx1280.setSyncWord(m_config.syncByte) != RADIOLIB_ERR_NONE) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Unable to set sync word!");
        return (false);
    }

    if (sx1280.setOutputPower(m_config.txPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected output power is invalid for this module!");
        return (false);
    }

    if (sx1280.setPreambleLength(m_config.preambleLength) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected preamble length is invalid for this module!");
        return (false);
    }

    if (sx1280.setCRC(m_config.crcEnabled) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: Selected CRC is invalid for this module!");
        return (false);
    }

    delay(1000);

	RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: setup complete");
	return (true);
}

size_t LoRaSX1280::sendPacket(std::vector<uint8_t> data)
{
	if (sx1280.transmit(data.data(), data.size()) == RADIOLIB_ERR_NONE)
	{
		RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: packet sent");
		return (data.size());
	} 
	RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("LoRa SX1280: transmit failed");
	return (0);
}

size_t LoRaSX1280::readPacket(std::vector<uint8_t>& data)
{
	if (!receivedFlag)
		return (0);
	receivedFlag = false;
	size_t len = sx1280.getPacketLength();
	data.resize(len);
	if (sx1280.readData(data.data(), len) == RADIOLIB_ERR_NONE)
		return (len);
	else
	{
		data.clear();
		return (0);
	}
}

bool LoRaSX1280::isBusy()
{
	return (false);
}

void LoRaSX1280::restart()
{}

float 	LoRaSX1280::calculateAirtime(size_t payloadSize)
{
	return (sx1280.getTimeOnAir(payloadSize)/1e6f);
}
