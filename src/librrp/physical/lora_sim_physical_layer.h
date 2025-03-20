#pragma once
#include "physical_layer_base.h"

// std
#include <deque>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>
#include <cmath>
#include <algorithm>

// ric
#include <libriccore/riccorelogging.h>
#include "radio_channel_manager.h"
#include "radio_channel.h"

struct LoRaSimPhysicalLayerInfo : public PhysicalLayerInfo {
    float frequency;       // Frequency in Hz
    float bandwidth;       // Bandwidth in Hz
    uint8_t spreadingFactor; // Spreading Factor (SF)
    uint8_t codingRate;     // Coding rate (denominator, e.g., 5 for 4/5)
    uint8_t preambleLength; // Preamble length (default: 8)
    bool crcEnabled;        // CRC enabled or not
    bool implicitHeader;    // Implicit header mode or not
    bool lowDataRateOptimization; // Low data rate optimization flag
};

class LoRaSimPhysicalLayer : public PhysicalLayerBase {

    public:
		LoRaSimPhysicalLayer(float frequency, float bandwidth, uint8_t spreadingFactor, uint8_t codingRate = 1, uint8_t preambleLength = 8, 
			bool crcEnabled = true, bool implicitHeader = false, bool lowDataRateOptimization = false);
        ~LoRaSimPhysicalLayer() override = default;
        bool setup() override;
        size_t sendPacket(std::vector<uint8_t> data) override;
        size_t readPacket(std::vector<uint8_t>& data) override;
        bool isBusy() override;
        void restart() override;
		float calculateAirtime(size_t payloadSize) const;
		const PhysicalLayerInfo* getInfo() override {return &m_info;}
		void setChannel(int newChannel);
		void pushToRxBuffer(std::vector<uint8_t> data);

    protected:

        std::queue<std::vector<uint8_t>> m_rxBuffer;
		LoRaSimPhysicalLayerInfo m_info;

		int m_currentChannel = -1;
		static RadioChannelManager radioChannelManager;
};