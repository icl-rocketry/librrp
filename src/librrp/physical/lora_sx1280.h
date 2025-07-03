#pragma once

#include "physical_layer_base.h"

#include <cmath>
// #include <Preferences.h>
#include <RadioLib.h>
#include <SPI.h>

struct LoRaSX1280LayerInfo : public PhysicalLayerInfo
{
};

struct LoRaSX1280Config
{
    float frequency;       // Frequency in Hz
    float bandwidth;       // Bandwidth in Hz
    uint8_t spreadingFactor; // Spreading Factor (SF)
    uint8_t codingRate;     // Coding rate (denominator, e.g., 5 for 4/5)
	uint8_t syncByte;
	int8_t txPower; //in dB
	uint16_t preambleLength; // Preamble length (default: 8)
    bool crcEnabled;        // CRC enabled or not
    bool implicitHeader;    // Implicit header mode or not
};

class LoRaSX1280 : public PhysicalLayerBase
{
	public:
		LoRaSX1280(int cs, int irq, int rst, int gpio, SPIClass& spi);
		~LoRaSX1280() override = default;
		bool 	setup() override;
        size_t	sendPacket(std::vector<uint8_t> data) override;
        size_t	readPacket(std::vector<uint8_t>& data) override;
        bool	isBusy() override;
        void	restart() override;
		float 	calculateAirtime(size_t payloadSize);
		const PhysicalLayerInfo* getInfo() override {return &m_info;}
		void setChannel(uint8_t channel){};

	private:

		static void setFlag(void){receivedFlag = false;};
		static volatile bool receivedFlag;
		LoRaSX1280LayerInfo m_info;
		LoRaSX1280Config m_config;
		LoRaSX1280Config m_defaultConfig{static_cast<float>(2400.0),
			static_cast<float>(812.5),
			static_cast<uint8_t>(12),
			static_cast<uint8_t>(5),
			static_cast<uint8_t>(0xF3),
			static_cast<int8_t>(13),
			static_cast<uint16_t>(16),
			false,
			false};
		Module module;
		SX1280 sx1280;
		SPIClass spi;
};